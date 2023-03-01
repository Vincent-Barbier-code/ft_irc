/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:44:47 by vbarbier          #+#    #+#             */
/*   Updated: 2023/03/01 20:46:48 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include <set>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: ./server <port>" << std::endl;
		exit(EXIT_FAILURE);
	}

	int port = atoi(argv[1]);

	Server server(port);
	server.start();
    return 0;
}
