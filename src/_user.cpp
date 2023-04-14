#include "Server.hpp"
#include "Client.hpp"

void Client::user(std::string const & username, std::string const & hostname,
                  std::string const & servername, std::string realname) {

    if (realname.size() > 50)
        clerr(ERR_NEEDMOREPARAMS);
    else if (_isRegistered)
        clerr(ERR_ALREADYREGISTRED);
    else if (_nickName.empty())
        clerr(ERR_NONICKNAMEGIVEN);
    
    _userName   = username;
    _hostName   = hostname;
    _serverName = servername;
    _realName   = realname;
    _isRegistered = true;
}

void Server::_user(Client & client, std::vector<std::string> const & params) {
    client.user(params[0], params[1], params[2], params[3]);
    _sendWelcomeMsg(client);
}

void 		Server::_sendWelcomeMsg(Client const & client) {
	std::cerr << "Welcoming: "; 
    _sendNumericReply(RPL_WELCOME, client);

}