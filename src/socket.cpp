/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:46:33 by vbarbier          #+#    #+#             */
/*   Updated: 2023/02/21 21:06:59 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

// bind lie le socket a une adresse et un port
socketServer::socketServer() {
	sin = get_addr();
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(DEFAULT_PORT);
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		std::cerr << "socket: error initialization" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bind(socket_fd, (sockaddr *)&sin, sizeof(sin))) {
		std::cerr << "bind: error initialization" << std::endl;
		exit(EXIT_FAILURE);
	}
}

// Constructeur qui prend en parametre le port sur lequel on veut creer le
// socket et l'adresse IP de la machine hote. Le port de l'adresse IP du serveur
// est stocké dans la structure sin avec la ligne sin.sin_port = htons(port);
socketServer::socketServer(int port) {
	sin = get_addr();
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		std::cerr << "socket: error initialization" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bind(socket_fd, (sockaddr *)&sin, sizeof(sin))) {
		std::cerr << "bind: error initialization" << std::endl;
		exit(EXIT_FAILURE);
	}
}

socketServer::~socketServer() {
}

// Recupere l'adresse IP de la machine hote
// sin est la structure qui contient l'adresse IP
sockaddr_in socketServer::get_addr() {
	struct addrinfo hints;
	struct addrinfo *res;
	int err;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	err = getaddrinfo(NULL, itostr(DEFAULT_PORT).c_str(), &hints, &res);
	if (err != 0) {
		std::cerr << "getaddrinfo: Error" << std::endl;
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in *sa = (struct sockaddr_in *)res->ai_addr;
	sin.sin_addr = sa->sin_addr;

	freeaddrinfo(res);
	return (sin);
}

// Converti un int en string
std::string itostr(int i) {
	std::string s;

	while (i > 0) {
		s = (char)(i % 10 + '0') + s;
		i /= 10;
	}
	return (s);
}