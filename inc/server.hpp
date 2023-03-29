/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:58:56 by vbarbier          #+#    #+#             */
/*   Updated: 2023/03/29 20:50:10 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
# pragma once

#include "macro.hpp"
#include "ft_irc.hpp"
#include "utils.hpp"
#include "client.hpp"
#include "Message.hpp"
#include <iomanip>
#include "Channel.hpp"

class Client;
class Channel;

class  Server {

  public:
	
	static void	sendData(Client const & client, std::string const & data);

	Server(int port, std::string password);
	~Server();
	
	int 		getFd() const;
	sockaddr_in getAddr() const;
	std::string	getPass() const;
	void 		start(void);
	void 		stop(void);

	void 		addChannel(Channel const & channel);

  private:
  	int							_server_fd;
	sockaddr_in					_addr;
	int							_epoll_fd;
	std::map<int, Client *>		_clients;
	std::string					_password;
	std::map<std::string, Channel>	_channels;
	
	Server();

	void		_initEpoll(void);
	void		_acceptNewConnection(void);
	void		_treatClientEvent(epoll_event const & client_ev);
	void		_deconnection(int client_fd);
	Client		*_findClientByFd(int fd);
	Client		*_findClientByNickName(std::string const nickName);
	int			_clientConnect(int client_fd, sockaddr client_addr, std::string username, std::string nickname);
	void		_execRawMsgs(std::string const & raw_msgs, int client_fd);

	std::string _getNumericReplyMessage(int code, Client const &client) const;
	void		_sendNumericReply(int code, Client const & client);
	void 		_sendMsgToCLient(Client const & client, std::string const & msg);
	void 		_sendWelcomeMsg(Client const & client);

	//channel
	void    	_createChannel(std::string const & channelName, std::string const & topic, Client const & client);


};
#endif

