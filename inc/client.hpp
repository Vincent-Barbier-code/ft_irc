
#ifndef CLIENT_HPP
#define CLIENT_HPP
# pragma once

#include "ft_irc.hpp"

class  Client {

  public:
	Client(int fd, sockaddr_in addr, std::string userName);
	~Client();
	
	int 		getFd();
	sockaddr_in getAddr();
	std::string getUserName();
	bool		isConnected() const;
	bool		isLoggedIn() const;
	//channel 		getCurrentChannel();
	//channel 		joinChannel(const std::string &channel_name);
	//channel 		leaveChannel();
	//void 			sendMsg(const std::string &msg);
	//void 			sendPrivateMsg(const std::string &msg);
	//void 			updateProfile();
	//void 			addContact();
	//void 			removeContact();
	//void 			connect();
	//void 			disconnect();


	
  private:
	int					_fd;
	struct sockaddr_in	_addr;
	std::string			_userName;
	std::string			_currentChannel;
	bool				_isLoggedIn;
	
	
	Client();
};
#endif

typedef Client* ClientPtr;
