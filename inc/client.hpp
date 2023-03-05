
#ifndef CLIENT_HPP
#define CLIENT_HPP
# pragma once

#include "ft_irc.hpp"

class  Client {

  public:
	Client(int fd);
	~Client();
	
	int 		getFd();
	sockaddr_in getAddr();
	std::string getUserName();

	
  private:
	int					_fd;
	struct sockaddr_in	_addr;
	std::string			_user_name;
	
	Client();
};
#endif

typedef Client* ClientPtr;