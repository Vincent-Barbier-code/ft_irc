#include "client.hpp"

Client::Client(): _fd(0), _addr(), _nickName(), _userName(), _isRegistered(false), _isAuth(false)
{
}

Client::Client(int fd, sockaddr_in addr, std::string userName, std::string nickName)
{
	_fd = fd;
	_addr = addr;
	_userName = userName;
	_nickName = nickName;
	_isRegistered = false;
}

Client::~Client()
{
}

int Client::getFd() const
{
	return (this->_fd);
}

sockaddr_in Client::getAddr()
{
	return (this->_addr);
}

std::string Client::getUserName() const
{
	return (this->_userName);
}

std::string Client::getNickName() const
{
	return (this->_nickName);
}

bool Client::isConnected() const {
	int error = 0;
	socklen_t len = sizeof(error);
	if (getsockopt(_fd, SOL_SOCKET, SO_ERROR, &error, &len) != 0) {
		return false;
	}
	if (error != 0)
		return false;
    return true;
}

bool Client::isRegistered() const {
	return (_isRegistered);
}

//Commande : PASS
//Paramètres : <password>
void	Client::pass(std::string const &clientPass, std::string const &serverPass) {
	if (_isAuth)
		throw (ERR_ALREADYREGISTRED);
	else if (clientPass == serverPass) {
		_isAuth = true;
	}
	throw (ERR_PASSWDMISMATCH);
}

//Commande : NICK
// Paramètres : <nicknamenyme>
void	Client::nick(std::string const nick, const Client *client)
{
	if (client)
		throw (ERR_NICKNAMEINUSE);
	if (MAX_NICKNAME_LENGTH > 9)
		throw (ERR_ERRONEUSNICKNAME);
	else if (!nick.size())
		throw (ERR_NONICKNAMEGIVEN);
	else
		_nickName = nick;
}

//commande : QUIT
//Paramètres : [ <Quit message> ]
void	Client::quit() {
	_isRegistered = false;
}