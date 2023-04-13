#include "Server.hpp"


void	Server::_nick(int client_fd, std::string const nick)
{
	Client & client = *_clients.at(client_fd);

	if (_findClientByNickName(nick))
	{
		size_t end;
		std::string new_nick = nick;

		int _nickNumber = 1;
		while (_findClientByNickName(new_nick)) {
			end = new_nick.find_last_not_of("0123456789");
			new_nick = new_nick.substr(0, end + 1);
			new_nick += itostr(_nickNumber);
			_nickNumber++;
		}

		for(std::map<int, Client *>::iterator receiver = _clients.begin(); receiver != _clients.end(); receiver++)
		 		client.sendMsgToClient(*receiver->second, "NICK " + new_nick);
		client.setNickName(new_nick);
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