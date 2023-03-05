/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:58:56 by vbarbier          #+#    #+#             */
/*   Updated: 2023/03/03 17:29:35 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
# pragma once

#include "client.hpp"
class  Server {

  public:
	Server(int port);
	~Server();
	
	int 		getFd();
	sockaddr_in getAddr();
	void 		start(void);
	void		shutdown();
	
  private:
	int					_server_fd;
	struct sockaddr_in	_addr;

	std::map<int, ClientPtr> _clients;
	
	
	Server();
};
#endif

