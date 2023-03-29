#include "server.hpp"
#include "macro.hpp"
#include "utils.hpp"

std::string Server::_getNumericReplyMessage(int code, Client const &client) const{
	std::string message = "";
	switch (code)
	{
		case RPL_WELCOME:
			message = "001 " + client.getNickName() + " :Welcome on the Marine, Vincent and Kevin IRC Server, my dare " + client.getNickName();
			break;
		case RPL_YOURHOST:
			message = "002 " + client.getNickName() + " :Your host is " + client.getNickName();
			break;
		case RPL_CREATED:
			message = "003 " + client.getNickName() + " :This server was created " + __DATE__ + " at " + __TIME__;
			break;
		case RPL_MYINFO:
			message = "004 " + client.getNickName() + " " + client.getServerName() ;//+ " " + USER_MODES + " " + CHANNEL_MODES;
			break;
		case RPL_LUSERCLIENT:
			message = "251 " + client.getNickName() + " :There are " + itostr(this->_clients.size()) + " users and " + " 1 server";
			break;
		// case RPL_LUSEROP:
		// 	message = "252 " + client.getNickName() + " " + itostr(this->_operator.size()) + " :operator(s) online";
		// 	break;
		// case RPL_LUSERCHANNELS:
		// 	message = "254 " + client.getNickName() + " " + itostr(this->getChannels().size()) + " :channels formed";
		// 	break;
		// case RPL_MOTD:
		// 	message = "372 " + client.getNickName() + " :- " + client.getServer()->getMotd();
		// 	break;
		case RPL_MOTDSTART:
			message = "375 " + client.getServerName() + " :- Message of the day -";
			break;
		case RPL_ENDOFMOTD:
			message = "376 :End of /MOTD command.";
			break;
		case ERR_NONICKNAMEGIVEN:
			message = "431 :No nickname given";
			break;
		case ERR_BADCHANNELKEY:
			message = "475 " + client.getNickName() + " :Cannot join channel wrong password";
			break;
	}
	return (message);
}