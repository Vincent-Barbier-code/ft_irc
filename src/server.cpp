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

void Server::startOld(){
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


void Server::start() {
	if (listen(server_fd, SOMAXCONN) == -1) {
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "listen..." << std::endl;

	_initEpoll();

	while (1) {
		epoll_event ready_events;
		int maxevents = 12;//a remplacer par le nombre de client + 1 une fois le merge effectue
		int nb_ev;

		//std::cout << "epollfd : " << epoll_fd << std::endl;

		if ((nb_ev = epoll_wait(epoll_fd, &ready_events, maxevents, 50000)) == -1) {
			perror("epoll_wait() failed first");
			exit(EXIT_FAILURE);
		}

		// on parcourt chaque event qui a ete triggered	
		for (int i = 0; i < nb_ev; i++) {

			if ((&ready_events)[i].data.fd == server_fd) {  //le socket du server a ete triggered car nouvelle connexion
				int 		client_fd;
				sockaddr	client_addr;
				socklen_t 	client_addrlen = sizeof(client_addr);
				memset(&client_addr, 0, client_addrlen);

				if ((client_fd = accept(server_fd, &client_addr, &client_addrlen)) == -1) {
					perror("accept() failed");
					exit(EXIT_FAILURE);
				}

				std::cout << "servererrre" << std::endl;

				epoll_event client_ev;
				client_ev.data.fd = client_fd;
				client_ev.events = EPOLLIN | EPOLLRDHUP;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev) == -1) {
					perror("epoll_ctl() failed");
					exit(EXIT_FAILURE);
				}
			} else { //un event sur un client a ete triggered (nouveau msg ou deconnexion)
				_treat_client_event((&ready_events)[i]);
			}
		}
	}

}

void Server::_initEpoll() {
	if ((epoll_fd = epoll_create(180)) == -1) {
		perror("epoll_create() failed");
		exit(EXIT_FAILURE);
	}

	epoll_event server_ev;

	server_ev.data.fd = server_fd;
	server_ev.events = EPOLLIN;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &server_ev) == -1) {
	   perror("epoll_ctl on server fd failed:");
	   exit(EXIT_FAILURE);
	}
}

void Server::_treat_client_event(epoll_event const & client_ev) const {
	int size = 1024;
	char buf[size + 1];
	bzero(buf, size + 1);

	if (client_ev.events & EPOLLRDHUP) {
		_deconnection(client_ev.data.fd);
		return ;
	}

	int len;
	int client_fd = client_ev.data.fd;
	if ((len = read(client_fd, buf, size)) == -1) { // A METTRE DANS UNE BOUCLE
		perror("read() failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "len ::  " << len << std::endl;
	buf[len] = '\0';
	std::cout << "client said : " << buf << std::endl;
}

void Server::_deconnection(int client_fd) const { // FONCTION A MODIFIER 
	std::cout << "Le client : " << client_fd << " a ete deconnecte !" << std::endl;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) == -1) {
		perror("epoll_ctl() failed");
		exit(EXIT_FAILURE);
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

