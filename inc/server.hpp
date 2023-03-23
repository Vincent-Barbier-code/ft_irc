/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:58:56 by vbarbier          #+#    #+#             */
/*   Updated: 2023/03/23 17:00:13 by vbarbier         ###   ########.fr       */
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
	Server(int port);
	~Server();
	
	int 		getFd();
	sockaddr_in getAddr();
	void 		start(void);
	void 		startOld(void);
	void 		stop(void);


  private:
  	int							_server_fd;
	sockaddr_in					_addr;
	int							_epoll_fd;
	std::map<int, ClientPtr>	_clients;
	
	
	Server();

	void	_initEpoll(void);
	void	_acceptNewConnection(void);
	void	_treatClientEvent(epoll_event const & client_ev);
	void	_deconnection(int client_fd);
	void	_execRawMsgs(std::string const & raw_msgs);
	int		_clientConnect(int client_fd, sockaddr client_addr);
};
#endif

