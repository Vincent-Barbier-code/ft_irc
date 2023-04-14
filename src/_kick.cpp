#include "Server.hpp"

bool Server::_isClientNameInList(Channel channel, std::string clientName) const {
	std::vector<int> userList = channel.getUserList();
	int client_fd;

	for (size_t i = 0; i < userList.size(); i++) {
		client_fd = userList.at(i);
		if (_clients.at(client_fd)->getNickName() == clientName)
			return true;
	}
	return false;
}

void Server::_kick(int client_fd, std::string const & channelName, std::string const banName, std::string const & comment)
{
	Client &client = *_clients.at(client_fd);

	if (client == *_findClientByNickName(banName))
		return ;
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