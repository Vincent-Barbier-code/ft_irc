#include "ft_irc.hpp"

Server::Server() {
}

Server::Server(int port){
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &addr, sizeof(addr))) {
		perror("setsockopt()");
		exit(EXIT_FAILURE);
	}

    if (bind(server_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }
}

void Server::start(){
	if (listen(server_fd, SOMAXCONN) == -1) {
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "listen..." << std::endl;
    while (1) {
        int			client_fd;
        sockaddr	client_addr;
        socklen_t	client_addrlen = sizeof(client_addr);
	
        memset(&client_addr, 0, client_addrlen);
		if ((client_fd = accept(server_fd, &client_addr, &client_addrlen)) == -1) 
			perror("accept() failed");
		std::cout << reinterpret_cast<void *>(&client_addr) << std::endl;
/*
		if (1)
		{
			close(client_fd);
			exit(EXIT_SUCCESS);
		}
		*/

       std::cout << "Client connected !!!" << std::endl;
    }
}

void	Server::shutdown()
{

}

int Server::getFd(){
	return(server_fd);
}

sockaddr_in Server::getAddr(){
	return(addr);
}


Server::~Server(){
}

