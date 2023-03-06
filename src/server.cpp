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

       std::cout << "Client connected, fd:" << client_fd << std::endl;

	  char buf[1024];
	   if (recv(client_fd, buf, sizeof(buf), 0) == -1) {
			perror("recv() failed");
			exit(EXIT_FAILURE);
	   }
	   std::cout << "Message du client: " << buf << std::endl;
	   char msg[] = "Je suis un messager du serveur !";
	   if(send(client_fd, msg, strlen(msg) + 1, 0) == -1) {
			perror("send failed");
			exit(EXIT_FAILURE);
	   }
	   	
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

