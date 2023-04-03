#include "server.hpp"

void Server::_parseJoin(int client_fd, std::string const & name)
{
	(void)(client_fd);
	if (name.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	if (name[0] != '#' && name[0] != '&')
		clerr(ERR_NOSUCHCHANNEL);
	if (name.size() > 200)
		clerr(ERR_NOSUCHCHANNEL);
	if (name.find(',') != std::string::npos || name.find(' ') != std::string::npos)
		clerr(ERR_NOSUCHCHANNEL);
}

std::string Server::_getUserNameList(Channel channel) const
{
    std::string userList = "";
	int client_fd;

    for (size_t i = 0; i < channel.getUserList().size(); i++) {
		client_fd = channel.getUserList().at(i);
		std::cout << "----------------------------client_fd: " << client_fd << std::endl;
        userList += " " + _clients.at(client_fd)->getNickName();
    }
    return userList;
}


void Server::_join(int client_fd, std::string const & name, std::string const & key)
{
	std::map<std::string, Channel>::iterator it2;

	_parseJoin(client_fd, name);
	it2 = _channels.find(name);
	if (it2 == _channels.end())
		addChannel(Channel(name, "" , *_clients.at(client_fd)));
	else
	{
		if (_channels.at(name).getkeyMask() == 1)
		{
			if (key != _channels.at(name).getPassword())
				clerr(ERR_BADCHANNELKEY);
		}
		// if mode invitation only, check if client is invited
		if (_channels.at(name).getinviteMask() == 1)
			if (!_channels.at(name).isInInviteList(client_fd))
				clerr(ERR_INVITEONLYCHAN);
		// if not banned
		if (_channels.at(name).getbanMask() == 1)
			if (_channels.at(name).isInBanList(client_fd))
				clerr(ERR_BANNEDFROMCHAN);
		_channels.at(name).addUser(client_fd);
	}
	_sendMsgToCLient(*_clients.at(client_fd), "JOIN " + name);
	_sendMsgToCLient(*_clients.at(client_fd), itostr(RPL_TOPIC) + " " + _clients.at(client_fd)->getNickName() + " " + name + " :" + _channels.at(name).getTopic());
	_sendMsgToCLient(*_clients.at(client_fd), itostr(RPL_NAMREPLY) + " " + _clients.at(client_fd)->getNickName() + " " + name + " Clients:" + _getUserNameList(_channels.at(name)));
}


void Server::_kick(std::string const & channelName, int client_fd, std::string const & comment)
{
	if (channelName.size() == 0 || comment.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	else
		clerr(ERR_CHANOPRIVSNEEDED);
	if (_channels.find(channelName) == _channels.end())
		clerr(ERR_NOSUCHCHANNEL);
	else if (!_channels.at(channelName).isInUserList(client_fd))
		clerr(ERR_NOTONCHANNEL);
	else if (_channels.at(channelName).isInBanList(client_fd))
		clerr(ERR_BANNEDFROMCHAN);
	if (_channels.at(channelName).isInOperatorList(client_fd))
	{
		_channels.at(channelName).removeUser(client_fd);
		_sendMsgToCLient(*_clients.at(client_fd), "KICK " + channelName + " " + _clients.at(client_fd)->getNickName() + " :" + comment);
	}
}

void Server::_invite(int client_fd, std::string const & nickName, std::string const & channelName)
{
	if (nickName.size() == 0 || channelName.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	if (!_findClientByNickName(nickName))
		clerr(ERR_NOSUCHNICK);
	if (_channels.at(channelName).isInOperatorList(client_fd))
	{
		_channels.at(channelName).addInvite(_findClientByNickName(nickName)->getFd());
		_sendMsgToCLient(*_clients.at(client_fd), "INVITE " + nickName + " " + channelName);
	}
	else
		clerr(ERR_CHANOPRIVSNEEDED);
}

void	Server::_nick(int client_fd, std::string const nick)
{
	if (_findClientByNickName(nick))
	{
		std::cout << "----------------------------- " + nick + " already exist" << std::endl;
		clerr(ERR_NICKNAMEINUSE);
	}
	else if (nick.size() > MAX_NICKNAME_LENGTH)
		clerr(ERR_ERRONEUSNICKNAME);
	else if (!nick.size())
		clerr(ERR_NONICKNAMEGIVEN);
	else
	{
		_clients.at(client_fd)->setNickName(nick);
		_sendMsgToCLient(*_clients.at(client_fd), "NICK " + nick);
	}
}


void Server::_part(int client_fd, std::string const & nameChannel)
{
	if (nameChannel.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	if (_channels.find(nameChannel) == _channels.end())
		clerr(ERR_NOSUCHCHANNEL);
	else if (!_channels.at(nameChannel).isInUserList(client_fd))
		clerr(ERR_NOTONCHANNEL);
	else
	{
		_channels.at(nameChannel).removeUser(client_fd);
		_sendMsgToCLient(*_clients.at(client_fd), "PART " + nameChannel);
	}
}

void	Server::_topic(int client_fd, std::string const & channelName, std::string const & topic)
{
	if (channelName.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	if (_channels.at(channelName).isInUserList(client_fd))
		clerr(ERR_NOTONCHANNEL);
	if (!_channels.at(channelName).isInOperatorList(client_fd))
		clerr(ERR_CHANOPRIVSNEEDED);	
	if (topic.size() == 0)
		clerr(RPL_NOTOPIC);
	_channels.at(channelName).setTopic(topic);
	clerr(RPL_TOPIC);

}
