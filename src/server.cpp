#include "server.hpp"

// ------------------ Server Class - Constructor / Destructor ------------------

int Server::_epoll_fd = -1;
std::map<int, std::string> Server::waitingData;

Server::Server() {
}

Server::~Server(){
}

Server::Server(int port, std::string password): _password(password) {
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1) {
		close(_server_fd);
		perror("socket()");
		exit(EXIT_FAILURE);
	}
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);

	_clients = std::map<int, ClientPtr>();

	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &_addr, sizeof(_addr))) {
		close(_server_fd);
		perror("setsockopt()");
		exit(EXIT_FAILURE);
	}

    if (bind(_server_fd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1) {
		close(_server_fd);
        perror("bind()");
        exit(EXIT_FAILURE);
    }
}


// ---------------------- Server starter ----------------------------

void Server::start() {
	if (listen(_server_fd, SOMAXCONN) == -1) {
		close(_server_fd);
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "listen..." << std::endl;
	displayClients();

	_initEpoll();

	while (!g_shutdown) {
		epoll_event ready_events;
		int			maxevents = _clients.size() + 1;
		int			nb_ev;

		if ((nb_ev = epoll_wait(_epoll_fd, &ready_events, maxevents, -1)) == -1) {
			if (errno == EINTR) //signal recu
				continue;
			_freeAndClose();
			perror("epoll_wait() failed first");
			exit(EXIT_FAILURE);
		}
		
		for (int i = 0; i < nb_ev; i++) {
			if ((&ready_events)[i].data.fd ==_server_fd)
				_acceptNewConnection();
			else
				_treatClientEvent((&ready_events)[i]);
		}
	}
	stop();
}

void Server::_initEpoll() {
	if ((_epoll_fd = epoll_create(1)) == -1) {
		perror("epoll_create() failed");
		_freeAndClose();
		exit(EXIT_FAILURE);
	}

	epoll_event server_ev;
	memset(&server_ev, 0, sizeof(server_ev));
	server_ev.data.fd =_server_fd;
	server_ev.events = EPOLLIN;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD,_server_fd, &server_ev) == -1) {
	   perror("epoll_ctl on server fd failed:");\
	   _freeAndClose();
	   exit(EXIT_FAILURE);
	}
}

// ---------------------- new client connection / deconnection ----------------------------

void Server::_acceptNewConnection(void) {
	int client_fd;

	sockaddr client_addr;
	socklen_t client_addrlen = sizeof(client_addr);
	memset(&client_addr, 0, client_addrlen);


	if ((client_fd = accept(_server_fd, &client_addr, &client_addrlen)) == -1) {
	   perror("accept() failed");
	   close(client_fd);
	   _freeAndClose();
	   exit(EXIT_FAILURE);
	}

	_clients[client_fd] = new Client(client_fd, reinterpret_cast<sockaddr_in &>(client_addr));
	
	displayClients();

	epoll_event client_ev;
	memset(&client_ev, 0, sizeof(client_ev));
	client_ev.data.fd = client_fd;
	client_ev.events = EPOLLIN | EPOLLRDHUP;

	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &client_ev) == -1)
	{
	   perror("epoll_ctl() failed");
	   _freeAndClose();
	   exit(EXIT_FAILURE);
	}
}

void Server::_deconnection(int client_fd) {
	std::cout << "Le client : " << client_fd << " a ete deconnecte !" << std::endl;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) == -1) {
		// MESSAGE LEAK
		perror("epoll_ctl() failed");
		_freeAndClose();
		exit(EXIT_FAILURE);
	}
	std::map<std::string, Channel>::iterator it = _channels.begin();

	for (_channels.begin(); it != _channels.end(); it++) {
		it->second.removeUser(client_fd);
	}
	close(client_fd);
	delete _clients[client_fd];
	_clients.erase(client_fd);
	
	displayClients();
}

// ---------------- Manage client events  ---------------------

void Server::_treatClientEvent(epoll_event const & client_ev) {
	
	std::string data;
	int 		size = 512;
	char 		buf[size + 1];
	memset(buf, 0, size + 1);
	
	int 	 len;
	int 	 client_fd = client_ev.data.fd;
	Client & client    = *_clients.at(client_fd);

	if (client_ev.events & EPOLLRDHUP) {
		_deconnection(client_ev.data.fd);
		return ;
	}
	if (client_ev.events & EPOLLOUT) {
		acceptSendData(client);
		return ;
	}

	if (client.getBuf().find("\r\n") != std::string::npos)
		client.clearBuf();

	len = read(client_fd, buf, size);
	if (len == -1) {
		_freeAndClose();
		perror("read() failed");
		exit(EXIT_FAILURE);
	}

	client.appendBuf(buf, len);
	if (client.getBuf().find("\r\n") != std::string::npos)
		_execRawMsgs(client.getBuf(), client_fd);
}

void Server::_execRawMsgs(std::string const & raw_msgs, int client_fd) {
	std::vector<Message> msgs = Message::parseAllMsg(raw_msgs);
	Client & client = *_clients.at(client_fd);

	for (std::vector<Message>::const_iterator it = msgs.begin(); it != msgs.end(); it++) {
		std::cout << "\n" << *it << std::endl;
		_execute(client, *it);
	}
}


// --------------------- Find_client -----------------

Client 	*Server::_findClientByNickName(std::string const nickName) const
{
	std::map<int, ClientPtr>::const_iterator it = _clients.begin();
	std::map<int, ClientPtr>::const_iterator ite = _clients.end();

	while(it != ite)
	{
		if (it->second->getNickName() == nickName)
			return (it->second);
		it++;
	}
	return (NULL);
}

Client 	*Server::_findClientByFd(int fd)
{
	std::map<int, ClientPtr>::iterator it = _clients.begin();
	std::map<int, ClientPtr>::iterator ite = _clients.end();

	while(it != ite)
	{
		if (it->first == fd)
			return (it->second);
		it++;
	}
	return (NULL);
}

bool	Server::_isClientOp(Channel const & chan, Client &client) {
	if (!chan.isClientInList(chan.getUserList(), client.getFd()))
		clerr(ERR_NOTONCHANNEL);
	if (!chan.isClientInList(chan.getOpList(), client.getFd()))
		clerr(ERR_CHANOPRIVSNEEDED);
	return (true);
}

// --------------------- Setters -----------------

// --------------------- Getters -----------------

int Server::getFd() const {
	return(_server_fd);
}

sockaddr_in Server::getAddr() const {
	return(_addr);
}

std::string	Server::getPass() const {
	return(_password);
}