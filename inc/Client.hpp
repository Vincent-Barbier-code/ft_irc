
#ifndef CLIENT_HPP
#define CLIENT_HPP
# pragma once

#include <list>
#include <set>
#include <algorithm>
#include "macro.hpp"
#include "Server.hpp"
#include "Channel.hpp"

class  Client {

  private:
	std::string 					_buf;
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
	bool							_isInvisible;
	
  public:

	typedef std::map<int, Client *> client_map;
	typedef std::map<std::string, Channel> channel_map;

	//---------Constructors - Destructors-----------
	Client();
	Client(int fd, sockaddr_in addr);
	Client(int fd, sockaddr_in addr, std::string nickName);
	~Client();

	bool operator==(Client const & rhs) const;
	
	//-------------------Getters--------------------
	int 		getFd() const;
	sockaddr_in getAddr();
	std::string getUserName() const; 
	std::string getNickName() const;
	std::string getHostName() const; 
	std::string getServerName() const;
	std::string getRealName() const;
	std::string getBuf() const;
	std::list<Channel const *>	getJoinedChannels(channel_map const & channels) const;
	std::set<Client const *>	getStalkers(channel_map const & channels, client_map const & clients) const;
	bool		isConnected() const;
	bool		isRegistered() const;
	bool 		isAuth() const;
	bool        isNicked() const;
	bool        isServerNamed() const;

	//----------------Setters----------------------
	std::string	setNickName(std::string const &nickName);
	void 		setBuf(std::string const & buf);
	void        appendBuf(char const * buf, size_t len);
	void 		clearBuf();

	//----------------Commands-----------------------
	void		pass(std::string const &clientPass, std::string const &serverPass);
	void 		user(std::string const & username, std::string const & hostname,
                  std::string const & servername, std::string realname);
	void		modeUser(std::string const name, std::string const mode, Client & client);
	
	//---------------Messages-----------------------
	void	botSendMsgToClient(Client const & client, std::string const & msg) const;
	void 	sendMsgToClient(Client const & client, std::string const & msg) const;
	void	sendMsgToClientsChannel(Channel const & channel, std::string const & msg, client_map const & clients, bool toMe) const;
	void 	sendMsgToStalkers(std::string const & msg, channel_map const & channels, client_map const & clients) const;
	void 	sendPrivateMsg(Client const & receiver, std::string const & msg, bool isNotice = false) const;
	void 	sendPrivateMsg(Channel const & channel, std::string const & msg, client_map const & clients) const;

	//------------------Bot--------------------
	std::string	_lookForMorse(std::string msg) const;
	void		_morse(Channel const &chan, std::string const & msg) const;

	//---------------Exceptions-----------------
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
