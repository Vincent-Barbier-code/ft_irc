#include "Server.hpp"

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
