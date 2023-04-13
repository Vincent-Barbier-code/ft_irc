#include "Server.hpp"

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
		for(std::map<int, Client *>::iterator receiver = _clients.begin(); receiver != _clients.end(); receiver++)
				client.sendMsgToClient(*receiver->second, "NICK " + nick);
		client.setNickName(nick);
	}
}