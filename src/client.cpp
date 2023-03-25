#include "client.hpp"
#include "struct.hpp"

Client::Client(): _fd(0), _addr(), _nickName(), _userName(), _isRegistered(false), _isAuth(false)
{
}

Client::Client(int fd, sockaddr_in addr, std::string userName, std::string nickName)
{
	try
	{
	_fd = fd;
	_addr = addr;
	_userName = userName;
	if (clientMap.find(nickName) != clientMap.end())
		throw NicknameInUseException();
	_nickName = nickName;
	_isRegistered = false;
	clientMap[_nickName] = *this;
	}
	catch (const NicknameInUseException& e) 
	{
		sendNumericReply(ERR_NICKNAMEINUSE);
	}
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
void Client::nick(std::string nickname){
	try 
	{
		std::string ancienNickname = _nickName;

		if (nickname.empty())
			throw InvalidNicknameException();
		if (nickname.size() > 9)
			throw InvalidNicknameException();
		if (clientMap.find(nickname) != clientMap.end())
		{
			// std::cout << "Nickname already in use" << std::endl;
			throw NicknameInUseException();
		}
		_nickName = nickname;
		sendNumericReply(RPL_WELCOME);
		if (!ancienNickname.empty())
			clientMap.erase(ancienNickname);
		clientMap[nickname] = *this;
	} 
	catch (const InvalidNicknameException& e)
	{
		sendNumericReply(ERR_ERRONEUSNICKNAME);
	}
	catch (const NicknameInUseException& e) 
	{
		sendNumericReply(ERR_NICKNAMEINUSE);
	}
}


void	Client::sendNumericReply(int code){
	std::string message = getNumericReplyMessage(code);
	std::cout << "Sending message to client " << _nickName << ": " << message << std::endl;
	
	//send MESSAGE KEVIN;
}

std::string	Client::getNumericReplyMessage(int code){
	switch (code){
		default:
			return "";
		case RPL_WELCOME:
			return _nickName + " :Welcome to the IRC server, " + _nickName + "!\r\n";
		case ERR_ERRONEUSNICKNAME:
			return "Erroneous nickname\r\n";
		case ERR_NICKNAMEINUSE:
			return _nickName + " :Nickname is already in use\r\n";
		case ERR_NEEDMOREPARAMS:
			return "<command> :Not enough parameters\r\n";
		case ERR_ALREADYREGISTRED:
			return ":You may not reregister\r\n";
	}
}

Client 	*findClientByNickName(std::string const nickName){
	if (clientMap.find(nickName) != clientMap.end())
		return &clientMap[nickName];
	return NULL;
}
