#include "server.hpp"

bool	g_shutdown = false;

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

// void Server::startOld(){
// 	if (listen(_server_fd, SOMAXCONN) == -1) {
//         perror("listen() failed");
//         exit(EXIT_FAILURE);
//     }


//     std::cout << "listen..." << std::endl;
//     while (1) {
// 		int			client_fd;
// 		sockaddr	client_addr;
// 		socklen_t	client_addrlen = sizeof(client_addr);

// 		memset(&client_addr, 0, client_addrlen);
// 		if ((client_fd = accept(_server_fd, &client_addr, &client_addrlen)) == -1) 
// 			perror("accept() failed");	


	
//         memset(&client_addr, 0, client_addrlen);
// 		if ((client_fd = accept(_server_fd, &client_addr, &client_addrlen)) == -1) 
// 			perror("accept() failed");

//        std::cout << "Client connected, fd:" << client_fd << std::endl;

// 	  char buf[1024];
// 	   if (recv(client_fd, buf, sizeof(buf), 0) == -1) {
// 			perror("recv() failed");
// 			exit(EXIT_FAILURE);
// 	   }
// 	   std::cout << "Message du client: " << buf << std::endl;
// 	   char msg[] = "Je suis un messager du serveur !";
// 	   if(send(client_fd, msg, strlen(msg) + 1, 0) == -1) {
// 			perror("send failed");
// 			exit(EXIT_FAILURE);
// 	   }
	   	
//     }
// }


void Server::start() {
	if (listen(_server_fd, SOMAXCONN) == -1) {
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "listen..." << std::endl;

	_initEpoll();

	while (!g_shutdown) {
		epoll_event ready_events;
		int maxevents = 12;//a remplacer par le nombre de client + 1 une fois le merge effectue
		int nb_ev;
		int 		nbr_client = 0;

		//std::cout << "epollfd : " << _epoll_fd << std::endl;

		if ((nb_ev = epoll_wait(_epoll_fd, &ready_events, maxevents, 50000)) == -1) {
			if (errno == EINTR) //signal recu
				continue;
			perror("epoll_wait() failed first");
			exit(EXIT_FAILURE);
		}

		// on parcourt chaque event qui a ete triggered	
		for (int i = 0; i < nb_ev; i++) {

			if ((&ready_events)[i].data.fd ==_server_fd) {  //le socket du server a ete triggered car nouvelle connexion
				int 		client_fd;
				sockaddr	client_addr;
				socklen_t 	client_addrlen = sizeof(client_addr);
				memset(&client_addr, 0, client_addrlen);

				if ((client_fd = accept(_server_fd, &client_addr, &client_addrlen)) == -1) {
					perror("accept() failed");
					exit(EXIT_FAILURE);
				}

				_clients[client_fd] = new Client(client_fd, reinterpret_cast<sockaddr_in &>(client_addr), "user_" + itostr(nbr_client), "nickname_" + itostr(nbr_client));
				nbr_client++;
				std::cout << "nbr client = " + itostr(nbr_client) << std::endl;
				std::cout << "Client connected " << _clients[client_fd]->getUserName() << std::endl;
				std::cout << "fd = " << client_fd << std::endl;
				std::cout << "nbr client connecte = " << _clients.size() << std::endl;

				std::cout << "servererrre" << std::endl;

				epoll_event client_ev;
				memset(&client_ev, 0, sizeof(client_ev));
				client_ev.data.fd = client_fd;
				client_ev.events = EPOLLIN | EPOLLRDHUP;
				if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev) == -1) {
					perror("epoll_ctl() failed");
					exit(EXIT_FAILURE);
				}
			} else { //un event sur un client a ete triggered (nouveau msg ou deconnexion)
				_treat_client_event((&ready_events)[i]);
			}
		}
	}
	stop();
}

void sigStop(int signum)
{
	(void)signum;
	std::cout << "Sigquit received." << std::endl;
	g_shutdown = 1;
}

void	Server::stop()
{
	std::map<int, ClientPtr>::iterator it = _clients.begin();
	std::map<int, ClientPtr>::iterator ite = _clients.end();

	std::cout << "Server is shutting down..." << std::endl;
	while(it != ite)
	{
		close(it->first); //first is used to access the key (fd of client)
		delete it->second; //second is used to access the value (pointer to client)
		_clients.erase(it++);
	}
	close(_epoll_fd);
	close(_server_fd);
	exit(EXIT_SUCCESS);
}

void Server::_initEpoll() {
	if ((_epoll_fd = epoll_create(180)) == -1) {
		perror("epoll_create() failed");
		exit(EXIT_FAILURE);
	}

	epoll_event server_ev;
	memset(&server_ev, 0, sizeof(server_ev));
	server_ev.data.fd =_server_fd;
	server_ev.events = EPOLLIN;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD,_server_fd, &server_ev) == -1) {
	   perror("epoll_ctl on server fd failed:");
	   exit(EXIT_FAILURE);
	}
}

void Server::_treat_client_event(epoll_event const & client_ev){
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

void Server::_deconnection(int client_fd){ // FONCTION A MODIFIER 
	std::cout << "Le client : " << client_fd << " a ete deconnecte !" << std::endl;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) == -1) {
		perror("epoll_ctl() failed");
		exit(EXIT_FAILURE);
	}
	close(client_fd);
	delete _client[client_fd];
	_clients.erase(client_fd);
}

int Server::getFd(){
	return(_server_fd);
}

sockaddr_in Server::getAddr(){
	return(_addr);
}


Server::~Server(){
}

