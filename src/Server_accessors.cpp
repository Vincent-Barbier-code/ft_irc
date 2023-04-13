#include "Server.hpp"

int Server::getFd() const {
	return(_server_fd);
}

sockaddr_in Server::getAddr() const {
	return(_addr);
}

std::string	Server::getPass() const {
	return(_password);
}