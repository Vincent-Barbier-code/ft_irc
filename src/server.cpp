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


void Server::start() {
	if (listen(_server_fd, SOMAXCONN) == -1) {
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "listen..." << std::endl;

	_initEpoll();

	while (!g_shutdown) {
		epoll_event ready_events;
		int			maxevents = _clients.size() + 1;
		int			nb_ev;

		if ((nb_ev = epoll_wait(_epoll_fd, &ready_events, maxevents, -1)) == -1) {
			if (errno == EINTR) //signal recu
				continue;
			perror("epoll_wait() failed first");
			exit(EXIT_FAILURE);
		}
		
		for (int i = 0; i < nb_ev; i++) {
			if ((&ready_events)[i].data.fd ==_server_fd)
				_acceptNewConnection();
			else
				_treat_client_event((&ready_events)[i]);
		}
	}
}

void Server::_initEpoll() {
	if ((_epoll_fd = epoll_create(1)) == -1) {
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

void Server::_acceptNewConnection(void) {

	static int nbr_client = 0;

	int client_fd;
	sockaddr client_addr;
	socklen_t client_addrlen = sizeof(client_addr);
	memset(&client_addr, 0, client_addrlen);

	if ((client_fd = accept(_server_fd, &client_addr, &client_addrlen)) == -1) {
	   perror("accept() failed");
	   exit(EXIT_FAILURE);
	}

	_clients[client_fd] = new Client(client_fd, reinterpret_cast<sockaddr_in &>(client_addr), "user_" + itostr(nbr_client), "nickname_" + itostr(nbr_client));
	nbr_client++;
	
	std::cout << "nbr client from begining= " + itostr(nbr_client) << std::endl;
	std::cout << "Client connected " << _clients[client_fd]->getUserName() << std::endl;
	std::cout << "fd = " << client_fd << std::endl;
	std::cout << "nbr client connecte = " << _clients.size() << std::endl;


	epoll_event client_ev;
	memset(&client_ev, 0, sizeof(client_ev));
	client_ev.data.fd = client_fd;
	client_ev.events = EPOLLIN | EPOLLRDHUP;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev) == -1)
	{
	   perror("epoll_ctl() failed");
	   exit(EXIT_FAILURE);
	}
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


void Server::_treat_client_event(epoll_event const & client_ev) {
	
	std::string data;
	int 		size = 512;
	char 		buf[size + 1];
	memset(buf, 0, size + 1);

	if (client_ev.events & EPOLLRDHUP) {
		_deconnection(client_ev.data.fd);
		return ;
	}

	int len;
	int client_fd = client_ev.data.fd;
	while ((len = read(client_fd, buf, size))) { // A METTRE DANS UNE BOUCLE
		if (len == -1) {
			perror("read() failed");
			exit(EXIT_FAILURE);
		}
		buf[len] = '\0';
		data.append(buf);
		if (0 && data.length() < 3) {
			std::cerr << "The server has received a message smaller than 3 character which is weird so i exit" << std::endl;
			exit(2);
		}
		if (data[data.length() - 2] == '\r' && data[data.length() - 1] == '\n')
			break;
	}
	//std::cout << "client said : " << data << "|end-cuicui| ";
	//fflush(stdout);
	
	_interpretData(data);

}

void Server::_interpretData(std::string const & data) {

	//std::cout << "client said : " << data << "|end-cuicui| ";
	//fflush(stdout);

	std::list<Message> msgs = Message::parseAllMsg(data);

	for (std::list<Message>::const_iterator it = msgs.begin(); it != msgs.end(); it++) 
		std::cout << "MSG: " << (*it).getRaw() << std::endl;


}

void Server::_deconnection(int client_fd){ // FONCTION A MODIFIER 
	std::cout << "Le client : " << client_fd << " a ete deconnecte !" << std::endl;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) == -1) {
		perror("epoll_ctl() failed");
		exit(EXIT_FAILURE);
	}
	close(client_fd);
	delete _clients[client_fd];
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

