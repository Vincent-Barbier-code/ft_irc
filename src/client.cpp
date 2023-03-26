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

int Client::getFd()
{
	return (this->_fd);
}

sockaddr_in Client::getAddr()
{
	return (this->_addr);
}

std::string Client::getUserName()
{
	return (this->_userName);
}

std::string Client::getNickName()
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
int	Client::pass(std::string const &clientPass, std::string const &serverPass) {
	if (_isAuth)
		return (ERR_ALREADYREGISTRED);
	else if (clientPass == serverPass) {
		_isAuth = true;
		return (0);
	}
	return (ERR_PASSWDMISMATCH);
}

//Commande : NICK
// Paramètres : <nicknamenyme>
int	Client::nick(std::string const nick)
{
	if (nick.size() > 9)
		return (ERR_ERRONEUSNICKNAME);
	else if (!nick.size())
		return (ERR_NONICKNAMEGIVEN);
	else {
		_nickName = nick;
		_isRegistered = true;
	}
}


