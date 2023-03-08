/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:58:56 by vbarbier          #+#    #+#             */
/*   Updated: 2023/03/07 15:32:38 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
# pragma once

#include "client.hpp"
#include "utils.hpp"
class  Server {

  public:
	Server(int port);
	~Server();
	
	int 		getFd();
	sockaddr_in getAddr();
	void 		start(void);
	void 		startOld(void);
	int					_server_fd;
	struct sockaddr_in	_addr;

  private:
	int			server_fd;
	int			epoll_fd;
	sockaddr_in	addr;
	std::map<int, ClientPtr> _clients;
	
	
	Server();

	void	_initEpoll(void);
	void	_treat_client_event(epoll_event const & client_ev) const;
	void	_deconnection(int client_fd) const;
};
#endif

