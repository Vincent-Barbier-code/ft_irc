/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:58:56 by vbarbier          #+#    #+#             */
/*   Updated: 2023/02/21 20:56:42 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class socketServer {
   private:
	int socket_fd;
	struct sockaddr_in sin;

   protected:
	socketServer();
	sockaddr_in get_addr();

   public:
	socketServer(int port);
	~socketServer();
	socketServer(socketServer const &src);
	socketServer &operator=(socketServer const &rhs);
	int initServerSocket(int port);
};

std::string itostr(int i);