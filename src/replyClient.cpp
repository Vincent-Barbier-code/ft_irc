#include "server.hpp"
#include "macro.hpp"
#include "utils.hpp"

void	Client::_sendNumericReply(int code){
	std::string message = _getNumericReplyMessage(code);
	std::cout << "Sending message to client " << _nickName << ": " << message << std::endl;
	
	//send MESSAGE KEVIN;
}

std::string Client::_getNumericReplyMessage(int code) const{
	std::string message = "";
	switch (code)
	{
		case RPL_WELCOME:
			message = "001 " + this->getNickName() + " :Welcome to the Internet Relay Network " + this->getNickName();
			break;
		case RPL_YOURHOST:
			message = "002 " + this->getNickName() + " :Your host is " + this->getNickName();
			break;
		case RPL_CREATED:
			message = "003 " + this->getNickName() + " :This server was created " + __DATE__ + " at " + __TIME__;
			break;
		// case RPL_MYINFO:
		// 	message = "004 " + this->getNickName() + " " + this->getServerName() + " " + USER_MODES + " " + CHANNEL_MODES;
		// 	break;
		// case RPL_LUSERCLIENT:
		// 	message = "251 " + this->getNickName() + " :There are " + itostr(this->getServer()->getClients().size()) + " users and " + " 1 server";
		// 	break;
		// case RPL_LUSEROP:
		// 	message = "252 " + this->getNickName() + " " + itostr(this->getServer()->getOperators().size()) + " :operator(s) online";
		// 	break;
		// case RPL_LUSERCHANNELS:
		// 	message = "254 " + this->getNickName() + " " + itostr(this->getServer()->getChannels().size()) + " :channels formed";
		// 	break;
		// case RPL_MOTD:
		// 	message = "372 " + this->getNickName() + " :- " + this->getServer()->getMotd();
		// 	break;
		case RPL_MOTDSTART:
			message = "375 " + this->getServerName() + " :- Message of the day -";
			break;
		case RPL_ENDOFMOTD:
			message = "376 :End of /MOTD command.";
			break;
		case ERR_NONICKNAMEGIVEN:
			message = "431 :No nickname given";
			break;
	}
	return (message);
}
