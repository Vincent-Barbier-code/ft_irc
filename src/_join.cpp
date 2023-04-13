#include "Server.hpp"

void Server::_parseJoin(std::string const & channelName)
{
	if (channelName.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	if (channelName[0] != '#' && channelName[0] != '&')
		clerr(ERR_NOSUCHCHANNEL);
	if (channelName.size() > 200)
		clerr(ERR_NOSUCHCHANNEL);
	if (channelName.find(',') != std::string::npos || channelName.find(' ') != std::string::npos)
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

int Server::_checkJoin(std::string const &channelName, int client_fd, std::string const & key)
{
    Client &client = *_clients.at(client_fd);
	Channel &channel = _channels.at(channelName);
	if (channel.getKeyMask() == 1)
	{
		if (key != channel.getPassword())
			clerr(ERR_BADCHANNELKEY);
	}
	// if mode invitation only, check if client is invited
	if (channel.getInviteMask() == 1) {
		if (!channel.isInInviteList(client_fd)) {
			client.sendMsgToClient(client, itostr(ERR_INVITEONLYCHAN) + " " + client.getNickName() + " " + channelName + " :Cannot join channel (+i)");
			return 0;
		}
		else
			channel.removeInvite(client_fd);
	}
	//if mode limit is set, check if channel is full
	if (channel.getUserLimitMask() && channel.getUserLimit() != 0)
		if (channel.getUserList().size() >= channel.getUserLimit()) {
			client.sendMsgToClient(client, itostr(ERR_CHANNELISFULL) + " " + client.getNickName() + " " + channelName + " :Cannot join channel (+l)");
			return 0;
	}
	// if not banned
	if (channel.isInBanList(client_fd))
	{
		client.sendMsgToClient(client, itostr(ERR_BANNEDFROMCHAN) + " " + client.getNickName() + " " + channelName + " :Cannot join channel (+b)");
		return 0;
	}
	channel.addUser(client_fd);
	if (channel.getOperatorList().size() == 0)
		channel.addOperator(client_fd);
    	std::string chan_mode;
	return 1;
}

void Server::_join(int client_fd, std::string const & channelName, std::string const & key)
{
    Client &client = *_clients.at(client_fd);

	_parseJoin(channelName);
	if (_channels.find(channelName) == _channels.end())
		addChannel(Channel(channelName, "" , client));
	else
		if (!_checkJoin(channelName, client_fd, key))
			return;

    std::string chan_mode;
	if (_channels.at(channelName).getPrivateMask() == 1)
		chan_mode = "*";
	else if (_channels.at(channelName).getSecretMask() == 1)
		chan_mode = "@";
	else
		chan_mode = "=";
	Channel &channel = _channels.at(channelName);

	client.sendMsgToClientsChannel(channel, "JOIN " + channelName, _clients, true);
	if (channel.getTopic().size() != 0)
		client.sendMsgToClient(client, itostr(RPL_TOPIC) + " " + client.getNickName() + " " + channelName + " :" + channel.getTopic());
	_sendMsgToClient(client, itostr(RPL_NAMREPLY) + " " + client.getNickName() + " " + chan_mode + " " + channelName + " :" + _getUserNameList(channel));
	_sendMsgToClient(client, itostr(RPL_ENDOFNAMES) + " " + client.getNickName() + " " + channelName);	
}