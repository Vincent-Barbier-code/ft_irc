
#ifndef CLIENT_HPP
#define CLIENT_HPP
# pragma once

#include "struct.hpp"

class  Client {


  public:
	Client();
	Client(int fd, sockaddr_in addr, std::string userName, std::string nickName);
	~Client();
	
	int 		getFd();
	sockaddr_in getAddr();
	std::string getUserName();
	std::string getNickName();
	bool		isConnected() const;
	bool		isLoggedIn() const;

	// channel 		getCurrentChannel();
	// channel 		joinChannel(const std::string &channel_name);
	// channel 		leaveChannel();
	// void 			sendMsg(const std::string &msg);
	// void 			sendPrivateMsg(const std::string &msg);
	// void 			updateProfile();
	// void 			addContact();
	// void 			removeContact();
	// void 			connect();
	// void 			disconnect();

	void		nick(std::string nick);

  private:
	int								_fd;
	struct sockaddr_in				_addr;
	std::string						_userName;
	std::string						_nickName;
	std::string						_currentChannel;
	bool							_isLoggedIn;

	void							_sendNumericReply(int code);
	std::string						_getNumericReplyMessage(int code);
};

	typedef Client* ClientPtr;

#endif
