
#ifndef CLIENT_HPP
#define CLIENT_HPP
# pragma once

#include "struct.hpp"

class  Client {

  private:
	int								_fd;
	struct sockaddr_in				_addr;
	std::string						_nickName;
	std::string						_userName;
	std::string 					_hostName;
	std::string 					_serverName;
	std::string						_realName;
	std::string						_currentChannel;
	bool							_isRegistered;
	bool							_isAuth;
	void							_sendNumericReply(int code);
	std::string						_getNumericReplyMessage(int code);
	
  public:
	Client();
	Client(int fd, sockaddr_in addr, std::string userName, std::string nickName);
	~Client();
	
	int 		getFd();
	sockaddr_in getAddr();
	std::string getUserName();
	std::string getNickName();
	std::string getHostName();
	std::string getServerName();
	std::string getRealName();
	bool		isConnected() const;
	bool		isRegistered() const;

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

	int			pass(std::string const &clientPass, std::string const &serverPass);
	void		nick(std::string nick);
	void 		user(std::string const & username, std::string const & hostname,
                  std::string const & servername, std::string realname);


};

	typedef Client* ClientPtr;

#endif
