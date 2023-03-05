#include "server.hpp"

Server::Server() {
}

Server::Server(int port){
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_clients = std::map<int, ClientPtr>();

	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_addr, sizeof(_addr))) {
		perror("setsockopt()");
		exit(EXIT_FAILURE);
	}

    if (bind(_server_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }
}

void Server::start(){
	if (listen(_server_fd, SOMAXCONN) == -1) {
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "listen..." << std::endl;
    while (1) {
		int			client_fd;
		sockaddr	client_addr;
		socklen_t	client_addrlen = sizeof(client_addr);
		int 		nbr_client = 0;

		memset(&client_addr, 0, client_addrlen);
		if ((client_fd = accept(_server_fd, &client_addr, &client_addrlen)) == -1) 
			perror("accept() failed");	

		_clients[client_fd] = new Client(client_fd);
		nbr_client++;
		std::cout << "Client connected on the fd : " << _clients[client_fd]->getFd() << std::endl;
		std::cout << "nbr client connecte = " << _clients.size() << std::endl;
		
    }
}

void	Server::shutdown()
{

}

int Server::getFd(){
	return(_server_fd);
}

sockaddr_in Server::getAddr(){
	return(_addr);
}


Server::~Server(){
}

