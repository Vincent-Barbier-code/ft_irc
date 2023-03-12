
#ifndef CLIENT_HPP
#define CLIENT_HPP
# pragma once

#include "ft_irc.hpp"

class  Client {

  public:
	Client(int fd, sockaddr_in addr, std::string userName, std::string nickName);
	~Client();
	
	int 		getFd();
	sockaddr_in getAddr();
	std::string getUserName();
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

	void		nick(std::string nick);//, std::string compteur);
	void		sendNumericReply(int code);
	std::string	getNumericReplyMessage(int code);
	Client		*findClientByNickName(std::string const nickName);
	class InvalidNicknameException : public std::exception {
	public:
		const char* what() const throw() {
			return "Invalid nickname";
		}
	};

	class NicknameInUseException : public std::exception {
	public:
		const char* what() const throw() {
			return "Nickname already in use";
		}
	};

	Client();
	
  private:
	int								_fd;
	struct sockaddr_in				_addr;
	std::string						_userName;
	std::string						_nickName;
	std::string						_currentChannel;
	bool							_isLoggedIn;
};

#endif

typedef Client* ClientPtr;
