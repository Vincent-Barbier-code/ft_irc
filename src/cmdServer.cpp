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

	_parseJoin(client_fd, name);
	if (_channels.find(name) == _channels.end())
		addChannel(Channel(name, "" , *_clients.at(client_fd)));
	else
	{
		Channel &channel = _channels.at(name);
		if (channel.getkeyMask() == 1)
		{
			if (key != channel.getPassword())
				clerr(ERR_BADCHANNELKEY);
		}
		// if mode invitation only, check if client is invited
		if (channel.getinviteMask() == 1) {
			if (!channel.isInInviteList(client_fd))
				clerr(ERR_INVITEONLYCHAN);
		}
		// if not banned
		if (channel.getbanMask() == 1)
			if (channel.isInBanList(client_fd))
				clerr(ERR_BANNEDFROMCHAN);
		channel.addUser(client_fd);
		if (channel.getUserList().size() == 1)
			channel.addOperator(client_fd);
	}
	Channel &channel = _channels.at(name);
	(*_clients.at(client_fd)).sendMsgToClientsChannel(channel, "JOIN " + name, _clients, true);
	(*_clients.at(client_fd)).sendMsgToCLient(*_clients.at(client_fd), itostr(RPL_TOPIC) + " " + _clients.at(client_fd)->getNickName() + " " + name + " :" + channel.getTopic());
	(*_clients.at(client_fd)).sendMsgToCLient(*_clients.at(client_fd),itostr(RPL_NAMREPLY) + " " + _clients.at(client_fd)->getNickName() + " " + name + " Clients:" + _getUserNameList(channel));
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
		std::cout << "----------------------------- " + nick + " is now your nickname" << std::endl;
		_clients.at(client_fd)->setNickName(nick);
		_sendMsgToCLient(*_clients.at(client_fd), "NICK " + nick);
	}
}


void Server::_part(int client_fd, std::string const & nameChannel)
{
	Client client = *_clients.at(client_fd);

	if (nameChannel.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	if (_channels.find(nameChannel) == _channels.end())
		clerr(ERR_NOSUCHCHANNEL);
	Channel &channel = _channels.at(nameChannel);

	if (!channel.isInUserList(client_fd))
		clerr(ERR_NOTONCHANNEL);
	else
	{
		client.sendMsgToClientsChannel(channel, "PART " + nameChannel, _clients, true);
		_sendMsgToCLient(client, "PART " + nameChannel);
		channel.removeUser(client_fd);
		// clerr(RPL_WELCOME);	
	}
}

void	Server::_topic(int client_fd, std::string const & channelName, std::string const & topic)
{
	if (channelName.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	else if (_channels.at(channelName).isInUserList(client_fd))
		clerr(ERR_NOTONCHANNEL);
	else if (!_channels.at(channelName).isInOperatorList(client_fd))
		clerr(ERR_CHANOPRIVSNEEDED);	
	else if (topic.size() == 0)
		_sendMsgToCLient(*_clients.at(client_fd), itostr(RPL_TOPIC) + " " + _clients.at(client_fd)->getNickName() + " " + channelName + " :" + _channels.at(channelName).getTopic());
	else
	{
		_channels.at(channelName).setTopic(topic);
		_sendMsgToCLient(*_clients.at(client_fd), "TOPIC " + channelName + " :" + topic);
	}

}
