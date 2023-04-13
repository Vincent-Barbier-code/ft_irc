#include "Server.hpp"

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
