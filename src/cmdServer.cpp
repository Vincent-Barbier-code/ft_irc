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
		if (channel.getOperatorList().size() == 0)
			channel.addOperator(client_fd);
	}
	Channel &channel = _channels.at(name);
	(*_clients.at(client_fd)).sendMsgToClientsChannel(channel, "JOIN " + name, _clients, true);
	_sendMsgToClient(*_clients.at(client_fd), itostr(RPL_TOPIC) + " " + _clients.at(client_fd)->getNickName() + " " + name + " :" + channel.getTopic());
	_sendMsgToClient(*_clients.at(client_fd),itostr(RPL_NAMREPLY) + " " + _clients.at(client_fd)->getNickName() + " " + name + " Clients:" + _getUserNameList(channel));
}


void Server::_kick(int client_fd, std::string const & channelName, std::string const banName, std::string const & comment)
{
	Client &client = *_clients.at(client_fd);

	if (channelName.size() == 0 || comment.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	if (_channels.find(channelName) == _channels.end())
		clerr(ERR_NOSUCHCHANNEL);
	Channel &channel = _channels.at(channelName);

	if (!channel.isInUserList(client_fd))
		clerr(ERR_NOTONCHANNEL);
	else if (channel.isInBanList(client_fd))
		clerr(ERR_BANNEDFROMCHAN);
	if (channel.isInOperatorList(client_fd))
	{
		channel.removeUser(_findClientByNickName(banName)->getFd());
		client.sendMsgToClientsChannel(channel, "KICK " + channelName + " " + _clients.at(client_fd)->getNickName() + " :" + comment, _clients, true);
	}
	else
		clerr(ERR_CHANOPRIVSNEEDED);
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
		_sendMsgToClient(*_clients.at(client_fd), "INVITE " + nickName + " " + channelName);
	}
	else
		clerr(ERR_CHANOPRIVSNEEDED);
}

void	Server::_nick(int client_fd, std::string const nick)
{
	Client &client = *_clients.at(client_fd);

	if (_findClientByNickName(nick))
	{
		int _nickNumber = 1;
		while (_findClientByNickName(nick + itostr(_nickNumber)))
			_nickNumber++;
		client.setNickName(nick + itostr(_nickNumber));
		clerr(ERR_NICKCOLLISION);
	}
	else if (nick.size() > MAX_NICKNAME_LENGTH)
		clerr(ERR_ERRONEUSNICKNAME);
	else if (!nick.size())
		clerr(ERR_NONICKNAMEGIVEN);
	else
	{
		client.setNickName(nick);
		_sendMsgToClient(client, "NICK " + nick);
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
		// _sendMsgToClient(client, "PART " + nameChannel);
		channel.removeUser(client_fd);
	}
}

void	Server::_topic(int client_fd, std::string const & channelName, std::string const & topic)
{
	if (_channels.find(channelName) == _channels.end())
		return ;
	Channel channel = _channels.at(channelName);
	Client client = *_clients.at(client_fd);

	if (channelName.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	else if (!channel.isInUserList(client_fd))
		clerr(ERR_NOTONCHANNEL);
	else if (!channel.isInOperatorList(client_fd))
		clerr(ERR_CHANOPRIVSNEEDED);	
	else if (topic.size() == 0)
		_sendMsgToClient(client, itostr(RPL_TOPIC) + " " + client.getNickName() + " " + channelName + " :" + channel.getTopic());
	else
	{
		channel.setTopic(topic);
		client.sendMsgToClientsChannel(channel, "TOPIC " + channelName + " :" + topic, _clients, true);
	}
}
