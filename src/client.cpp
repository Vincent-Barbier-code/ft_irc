#include "client.hpp"

Client::Client(int fd, sockaddr_in addr, std::string userName)
{
	this->_fd = fd;
	this->_addr = addr;
	this->_userName = userName;
	this->_isLoggedIn = true;
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

bool Client::isLoggedIn() const {
	return (this->_isLoggedIn);
}