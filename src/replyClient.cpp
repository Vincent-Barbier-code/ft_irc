#include "server.hpp"
#include "macro.hpp"
#include "utils.hpp"

std::string Server::_getNumericReplyMessage(int code, Client const &client) const{
	std::string message = "";
	switch (code)
	{
		case RPL_WELCOME:
			message = "001 " + client.getNickName() + " :Welcome on the Marine, Vincent and Kevin IRC Server, my dear " + client.getNickName();
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
		case ERR_NOSUCHNICK:
			message = "401 " + client.getNickName() + " :No such nick/channel";
			break;
		case ERR_NOSUCHCHANNEL:
			message = "403 :No such channel";
			break;
		case ERR_CANNOTSENDTOCHAN:
			message = "404 " + client.getNickName() + " :Cannot send to channel";
			break;
		case ERR_TOOMANYTARGETS:
			message = "407 " + client.getNickName() + " :Too many targets";
			break;
		case ERR_NORECIPIENT:
			message = "411 :No recipient";
			break;
		case ERR_NOTEXTTOSEND:
			message = "412 :No text to send";
			break;
		case ERR_NONICKNAMEGIVEN:
			message = "431 " + client.getNickName() + " :No nickname given";
			break;
		case ERR_ERRONEUSNICKNAME:
			message = "432 " + client.getNickName() + " :Erroneous nickname";
			break;
		case ERR_NICKNAMEINUSE:
			message = "433 " + client.getNickName() + " :Nickname is already in use";
			break;
		case ERR_NOTONCHANNEL:
			message = "442 :You're not on that channel";
			break;
		case ERR_NOTREGISTERED:
			message = "451 " + client.getNickName() + " :You have not registered";
			break;
		case ERR_NEEDMOREPARAMS:
			message = "461 " + client.getNickName() + " :Not enough parameters";
			break;
		case ERR_ALREADYREGISTRED:
			message = "462 " + client.getNickName() + " :You may not reregister";
			break;
		case ERR_PASSWDMISMATCH:
			message = "464 " + client.getNickName() + " :Password incorrect";
			break;
		case ERR_UNKNOWNMODE:
			message = "472 :Unknown mode";
			break;
		case ERR_INVITEONLYCHAN:
			message = "473 " + client.getNickName() + " :Cannot join channel (+i)";
			break;
		case ERR_BANNEDFROMCHAN:
			message = "474 " + client.getNickName() + " :Cannot join channel (+b)";
			break;
		case ERR_BADCHANNELKEY:
			message = "475 " + client.getNickName() + " :Cannot join channel wrong password";
			break;
		case ERR_CHANOPRIVSNEEDED:
			message = "482 " + client.getNickName() + " :You're not channel operator";
			break;
		case ERR_UMODEUNKNOWNFLAG:
			message = "501 :Unknown MODE flag";
			break;
		case ERR_USERSDONTMATCH:
			message = "503 :Cant change mode for other users";
			break;
		default:
			throw std::runtime_error("ERROR: Unknown numeric reply code");
	}
	return (message);
}