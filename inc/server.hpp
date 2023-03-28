/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:58:56 by vbarbier          #+#    #+#             */
/*   Updated: 2023/03/28 16:36:34 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
# pragma once

#include "utils.hpp"
#include "client.hpp"
#include "Message.hpp"
#include <iomanip>
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


  private:
  	int							_server_fd;
	sockaddr_in					_addr;
	int							_epoll_fd;
	std::map<int, ClientPtr>	_clients;
	std::string					_password;
	
	Server();

	void	_initEpoll(void);
	void	_acceptNewConnection(void);
	void	_treatClientEvent(epoll_event const & client_ev);
	void	_deconnection(int client_fd);
	Client	*_findClientByFd(int fd);
	Client	*_findClientByNickName(std::string const nickName);
	int		_clientConnect(int client_fd, sockaddr client_addr, std::string username, std::string nickname);
	void	_execRawMsgs(std::string const & raw_msgs, int client_fd);
	void	_sendMsgToCLient(Client const & client, std::string const & msg);
	void	_sendNumericReply(int code, Client const & client);
	std::string _getNumericReplyMessage(int code, Client const &client) const;

	void		_sendNumericReply(int code, Client const & client);
	void 		_sendMsgToCLient(Client const & client, std::string const & msg);
	void 		_sendWelcomeMsg(Client const & client);
};
#endif

