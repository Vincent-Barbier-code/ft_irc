/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:58:56 by vbarbier          #+#    #+#             */
/*   Updated: 2023/03/01 20:54:35 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# pragma once

class  Server {

  public:
	Server(int port);
	Server(Server const &src);
	~Server();
	
	//Server &operator=(Server const &rhs); A faire ou non?

	int 		getFd();
	sockaddr_in getAddr();
	void 		start(void);
	void 		startOld(void);
	void		shutdown();	

  private:
	int			server_fd;
	int			epoll_fd;
	sockaddr_in	addr;
	
	Server();

	void	_initEpoll(void);
	void	_treat_client_event(epoll_event const & client_ev) const;
	void	_deconnection(int client_fd) const;
};
#endif

