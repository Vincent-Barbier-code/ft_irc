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
// void	Client::nick(std::string nickname){
// 	try 
// 	{
// 		std::string ancienNickname = _nickName;

// 		if (nickname.empty())
// 			throw Exception();
// 		if (nickname.size() > 9)
// 			throw InvalidNicknameException();
// 		if (clientMap.find(nickname) != clientMap.end())
// 		{
// 			// std::cout << "Nickname already in use" << std::endl;
// 			throw NicknameInUseException();
// 		}
// 		_nickName = nickname;
// 		_isLoggedIn = true;
// 		sendNumericReply(RPL_WELCOME);
// 		if (!ancienNickname.empty())
// 			clientMap.erase(ancienNickname);
// 		clientMap[nickname] = *this;
// 	} 
// 	catch (const InvalidNicknameException& e)
// 	{
// 		sendNumericReply(ERR_ERRONEUSNICKNAME);
// 	}
// 	catch (const NicknameInUseException& e) 
// 	{
// 		sendNumericReply(ERR_NICKNAMEINUSE);
// 	}
// }

