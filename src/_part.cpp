#include "Server.hpp"

void Server::_part(int client_fd, std::string const & channelName)
{
	Client client = *_clients.at(client_fd);

	if (channelName.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	if (_channels.find(channelName) == _channels.end())
		clerr(ERR_NOSUCHCHANNEL);
	Channel &channel = _channels.at(channelName);
	if (!channel.isInUserList(client_fd))
		clerr(ERR_NOTONCHANNEL);
	else
	{
		client.sendMsgToClientsChannel(channel, "PART " + channelName, _clients, false);
		channel.removeUser(client_fd);
	}
}