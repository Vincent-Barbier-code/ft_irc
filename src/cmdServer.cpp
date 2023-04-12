#include "server.hpp"

void Server::_parseJoin(std::string const & name)
{
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
		if (i != 0)
            userList += " ";
        client_fd = channel.getUserList().at(i);
        if (channel.isInOperatorList(client_fd))
            userList += "@";
        else if (channel.isInVoiceList(client_fd))
            userList += "+";
        userList += _clients.at(client_fd)->getNickName();
    }
    return userList;
}

int Server::_checkJoin(std::string const &name, int client_fd, std::string const & key)
{
    Client &client = *_clients.at(client_fd);
	Channel &channel = _channels.at(name);
	if (channel.getKeyMask() == 1)
	{
		if (key != channel.getPassword())
			clerr(ERR_BADCHANNELKEY);
	}
	// if mode invitation only, check if client is invited
	if (channel.getInviteMask() == 1) {
		if (!channel.isInInviteList(client_fd)) {
			client.sendMsgToClient(client, itostr(ERR_INVITEONLYCHAN) + " " + client.getNickName() + " " + name + " :Cannot join channel (+i)");
			return 0;
		}
		else
			channel.removeInvite(client_fd);
	}
	//if mode limit is set, check if channel is full
	if (channel.getUserLimitMask() && channel.getUserLimit() != 0)
		if (channel.getUserList().size() >= channel.getUserLimit()) {
			client.sendMsgToClient(client, itostr(ERR_CHANNELISFULL) + " " + client.getNickName() + " " + name + " :Cannot join channel (+l)");
			return 0;
	}
	// if not banned
	if (channel.isInBanList(client_fd))
	{
		client.sendMsgToClient(client, itostr(ERR_BANNEDFROMCHAN) + " " + client.getNickName() + " " + name + " :Cannot join channel (+b)");
		return 0;
	}
	channel.addUser(client_fd);
	if (channel.getOperatorList().size() == 0)
		channel.addOperator(client_fd);
    	std::string chan_mode;
	return 1;
}


void Server::_join(int client_fd, std::string const & name, std::string const & key)
{
    Client &client = *_clients.at(client_fd);

	_parseJoin(name);
	if (_channels.find(name) == _channels.end())
		addChannel(Channel(name, "" , client));
	else
		if (!_checkJoin(name, client_fd, key))
			return;

    std::string chan_mode;
	if (_channels.at(name).getPrivateMask() == 1)
		chan_mode = "*";
	else if (_channels.at(name).getSecretMask() == 1)
		chan_mode = "@";
	else
		chan_mode = "=";
	Channel &channel = _channels.at(name);

	client.sendMsgToClientsChannel(channel, "JOIN " + name, _clients, true);
	if (channel.getTopic().size() != 0)
		client.sendMsgToClient(client, itostr(RPL_TOPIC) + " " + client.getNickName() + " " + name + " :" + channel.getTopic());
	_sendMsgToClient(client, itostr(RPL_NAMREPLY) + " " + client.getNickName() + " " + chan_mode + " " + name + " :" + _getUserNameList(channel));
	_sendMsgToClient(client, itostr(RPL_ENDOFNAMES) + " " + client.getNickName() + " " + name);
	
}

bool Server::_isClientNameInList(Channel channel, std::string name) const {
	std::vector<int> userList = channel.getUserList();
	int client_fd;

	for (size_t i = 0; i < userList.size(); i++) {
		client_fd = userList.at(i);
		if (_clients.at(client_fd)->getNickName() == name)
			return true;
	}
	return false;
}


void Server::_kick(int client_fd, std::string const & channelName, std::string const banName, std::string const & comment)
{
	Client &client = *_clients.at(client_fd);

	if (channelName.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	if (_channels.find(channelName) == _channels.end())
		clerr(ERR_NOSUCHCHANNEL);
	Channel &channel = _channels.at(channelName);

	if (!channel.isInUserList(client_fd))
		clerr(ERR_NOTONCHANNEL);
	else if (!_isClientNameInList(channel, banName))
		clerr(ERR_USERNOTINCHANNEL);
	else if (channel.isInBanList(client_fd))
		clerr(ERR_BANNEDFROMCHAN);
	if (channel.isInOperatorList(client_fd))
	{
		Client &clientBan = *_findClientByNickName(banName);
		clientBan.sendMsgToClient(clientBan, "PART " + channelName);
		client.sendMsgToClientsChannel(channel, "KICK " + channelName + " " + banName + " :" + comment, _clients, true);
		channel.removeUser(clientBan.getFd());
	}
	else
		clerr(ERR_CHANOPRIVSNEEDED);
}

void Server::_invite(int client_fd, std::string const & nickName, std::string const & channelName)
{
	if (_channels.find(channelName) == _channels.end())
		clerr(ERR_NOSUCHCHANNEL);
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
	Client & client = *_clients.at(client_fd);
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
		std::cout << "----------------------------- " + nick + " is now your nickname" << std::endl;
		client.sendMsgToStalkers("NICK " + nick, _channels, _clients);
		client.sendMsgToClient(client, "NICK " + nick);
		client.setNickName(nick);
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
		client.sendMsgToClientsChannel(channel, "PART " + nameChannel, _clients, false);
		channel.removeUser(client_fd);
	}
}

void	Server::_topic(int client_fd, std::string const & channelName, std::string const & topic)
{
	if (_channels.find(channelName) == _channels.end())
		return ;
	Channel &channel = _channels.at(channelName);
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
