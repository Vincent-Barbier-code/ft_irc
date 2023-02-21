/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:44:47 by vbarbier          #+#    #+#             */
/*   Updated: 2023/02/21 18:53:34 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: ./server <port>" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Initialisation du socket serveur
	int port = atoi(argv[1]);
	port = port + 1;
	// int sockfd = initServerSocket(port);

	return (0);
}