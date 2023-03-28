
#ifndef CLIENT_HPP
#define CLIENT_HPP
# pragma once

#include "ft_irc.hpp"
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
	
  public:
	Client();
	Client(int fd, sockaddr_in addr, std::string userName, std::string nickName);
	~Client();
	
	int 		getFd() const;
	sockaddr_in getAddr();
	std::string getUserName() const; 
	std::string getNickName() const;
	std::string getHostName() const; 
	std::string getServerName() const;
	std::string getRealName() const;
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
	int			nick(std::string const nick, Client const *client);
	int			quit();
	void 		user(std::string const & username, std::string const & hostname,
                  std::string const & servername, std::string realname);

	class ClientException : public std::exception {
		private:
			int _code;
		public:
			ClientException(int code) : _code(code) {};
			virtual const char* what() const throw() {
				return "ClientException";
			}
			int getCode() const throw() {
				return _code;
			}
	};
};

	typedef Client* ClientPtr;
	typedef Client::ClientException clerr;

#endif
