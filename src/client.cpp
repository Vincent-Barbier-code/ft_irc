#include "client.hpp"
#include "Channel.hpp"

Client::Client(): _fd(0), _addr(), _nickName(), _userName(), _isRegistered(false), _isAuth(false), _isInvisible(false)
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
		clerr(ERR_ALREADYREGISTRED);
	else if (clientPass == serverPass) {
		_isAuth = true;
	}
	clerr(ERR_PASSWDMISMATCH);
}

//Commande : NICK
// Paramètres : <nicknamenyme>
void	Client::nick(std::string const nick, const Client *client)
{
	if (client)
		clerr(ERR_NICKNAMEINUSE);
	else if (MAX_NICKNAME_LENGTH > 9)
		clerr(ERR_ERRONEUSNICKNAME);
	else if (!nick.size())
		clerr(ERR_NONICKNAMEGIVEN);
	else
		_nickName = nick;
}

//Commande : MODE
// Parametres 1 : <canal> {[+|-]|o|p|s|i|t|n|b|v} [<limite>] [<utilisateur>] [<masque de bannissement >]
// Parametres 2 : <pseudonyme> {[+|-]i}
void	Client::modeUser(std::string const name, std::string const mode) {
	if (name != _nickName)
		clerr(ERR_USERSDONTMATCH);
	if (mode[1] == 'i') {
		if (mode[0] == '+')
			_isInvisible = true;
		else
			_isInvisible = false;
	}
	else
		clerr(ERR_UMODEUNKNOWNFLAG);
}
