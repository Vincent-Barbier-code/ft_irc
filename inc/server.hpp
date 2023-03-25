/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvue <mvue@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:58:56 by vbarbier          #+#    #+#             */
/*   Updated: 2023/03/25 17:20:05 by mvue             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
# pragma once

#include "client.hpp"
#include "utils.hpp"
#include "Message.hpp"
#include <iomanip>
class  Server {

  public:
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
	void	_execRawMsgs(std::string const & raw_msgs, int client_fd);
	int		_clientConnect(int client_fd, sockaddr client_addr);
	void	_sendData(int client_fd, std::string const & data);
	void	_sendData(Client const & client, std::string const & data);
};
#endif

