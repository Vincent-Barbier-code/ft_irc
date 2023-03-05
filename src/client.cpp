#include "client.hpp"

Client::Client(int fd)
{
	this->_fd = fd;
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
	return (this->_user_name);
}

