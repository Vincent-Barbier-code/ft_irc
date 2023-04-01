#include "server.hpp"

bool	g_shutdown = false;


// ------------------ Server Class - Constructor / Destructor ------------------

Server::Server() {
}

Server::~Server(){
}

Server::Server(int port, std::string password): _password(password) {
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


// ---------------------- Server starter ----------------------------

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
				_treatClientEvent((&ready_events)[i]);
		}
	}
	stop();
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


// ---------------------- new client connection / deconnection ----------------------------

void Server::_acceptNewConnection(void) {
	int client_fd;
	sockaddr client_addr;
	socklen_t client_addrlen = sizeof(client_addr);
	memset(&client_addr, 0, client_addrlen);

	if ((client_fd = accept(_server_fd, &client_addr, &client_addrlen)) == -1) {
	   perror("accept() failed");
	   exit(EXIT_FAILURE);
	}

	static int i = 0;

	_clientConnect(client_fd, client_addr, "username" + itostr(i) , "nickname" + itostr(i));
	i++;
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

int		Server::_clientConnect(int client_fd, sockaddr client_addr, std::string username, std::string nickname)
{
	_clients[client_fd] = new Client(client_fd, reinterpret_cast<sockaddr_in &>(client_addr), username, nickname);

	// std::cout << "Client connected " << _clients.at(client_fd)->getNickName() << std::endl;
	// std::cout << "fd = " << client_fd << std::endl;
	// std::cout << "nbr client connecte = " << _clients.size() << std::endl;
	return (0);
}

void Server::_deconnection(int client_fd) {
	std::cout << "Le client : " << client_fd << " a ete deconnecte !" << std::endl;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) == -1) {
		perror("epoll_ctl() failed");
		exit(EXIT_FAILURE);
	}
	std::map<std::string, Channel>::iterator it = _channels.begin();

	for (_channels.begin(); it != _channels.end(); it++) {
		it->second.removeUser(client_fd);
	}
	close(client_fd);
	delete _clients[client_fd];
	_clients.erase(client_fd);
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


// ---------------- Manage client events  ---------------------

void Server::_treatClientEvent(epoll_event const & client_ev) {
	
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
	while ((len = read(client_fd, buf, size))) {
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
	
	_execRawMsgs(data, client_fd);
}

void Server::_execRawMsgs(std::string const & raw_msgs, int client_fd) {

	std::vector<Message> msgs = Message::parseAllMsg(raw_msgs);
	Client const & client = *_clients.at(client_fd);

	for (std::vector<Message>::const_iterator it = msgs.begin(); it != msgs.end(); it++) {
		//std::cout << "MSG: " << std::setw(50) << (*it).getRaw()  << "|   CMD: |" << (*it).getCmd() << "|" << std::endl;
		std::cout << *it << std::endl;
		std::string const & cmd = it->getCmd();
		std::vector<std::string> paramsV = it->getParamsValues();
		try {
            if (it->getErr()) {
				clerr(it->getErr());
			}
			else if (cmd == "NICK")
				_nick(client_fd, paramsV[0]);
			else if (cmd == "USER")
			{
				_clients.at(client_fd)->user(paramsV[0], paramsV[1], paramsV[2], paramsV[3]);

				_sendWelcomeMsg(*_clients.at(client_fd));
			}
			else if (cmd == "PASS")
				_clients.at(client_fd)->pass(paramsV[0], getPass());
			else if (cmd == "QUIT")
			{
				std::string quitMsg = paramsV.size() ? paramsV[0] : "Aurevoir !" + _clients.at(client_fd)->getNickName();
				_sendMsgToCLient(*_clients.at(client_fd), paramsV.size() ? paramsV[0] : quitMsg); // a PARSER
				// il faudra envoyer le message dans les canaux ou le client est present
				_deconnection(client_fd);
			}
			else if (cmd == "JOIN")
				_join(client_fd, paramsV[0], paramsV[1]);
			else if (cmd == "KICK")
				_kick(paramsV[0], client_fd, paramsV[1]);
			else if(cmd == "INVITE")
				_invite(client_fd, paramsV[0], paramsV[1]);
			else if (cmd == "LIST") {
				//addChannel(Channel("#general", "Welcome to the general channel", *_clients.at(client_fd)));
				//addChannel(Channel("#zizi", "Welcome to zizi channel", *_clients.at(client_fd)));
				_list(*_clients.at(client_fd));
			}
			else if (cmd == "PRIVMSG") {
				_sendPrivateMsg(client, paramsV[0], paramsV[1]);
			}
        }
			else if (cmd == "PING")
				_sendMsgToCLient(*_clients.at(client_fd), "PONG " + paramsV[0]);
		}
		catch(const Client::ClientException& e)
		{
			_sendNumericReply(e.getCode(), *_clients.at(client_fd));
		}
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

void	Server::_modeO(Channel & chan, std::string const mode, std::string const option) {
	std::vector<std::string> options;
	Client	*client_to_op;

	options = ke_split(option, ",");
	if (options.size() > 3)
		clerr(ERR_UNKNOWNMODE);
	std::vector<std::string>::iterator it;
	for (it = options.begin(); it != options.end(); it++) {
		if (!(*it).size())
			clerr(ERR_NEEDMOREPARAMS);
		client_to_op = _findClientByNickName(*it);
		if (!client_to_op)
			clerr(ERR_NOSUCHNICK);
		if (mode[0] == '+')
			chan.addClientToList(chan.getOpList(), client_to_op->getFd());
		else 
			chan.rmClientFromList(chan.getOpList(), client_to_op->getFd());
	}
}

void	Server::_modeB(Channel & chan, std::string const mode, std::string const option) {
	std::vector<std::string> options;
	Client	*client_to_ban;

	options = ke_split(option, ",");
	if (options.size() > 3)
		clerr(ERR_UNKNOWNMODE);
	std::vector<std::string>::iterator it;
	for (it = options.begin(); it != options.end(); it++) {
		if (!(*it).size())
			clerr(ERR_NEEDMOREPARAMS);
		client_to_ban = _findClientByNickName(*it);
		if (!client_to_ban)
			clerr(ERR_NOSUCHNICK);
		if (mode[0] == '+') {
			chan.addClientToList(chan.getBanList(), client_to_ban->getFd());
			//if (chan.isClientInList(chan.getUserList(), client_to_ban->getFd()))
			//	KICK_USER
		}
		else 
			chan.rmClientFromList(chan.getBanList(), client_to_ban->getFd());
	}
}

void	Server::_modeK(Channel & chan, std::string const mode, std::string const option) {
	for (int i = 0; option[i]; i++)
		if (isspace(option[i]))
			clerr(ERR_UNKNOWNMODE);
	if (mode[0] == '+') {
		chan.setKeyMask(true);
		if (option.size())
			chan.setKey(option);
		else 
			clerr(ERR_NEEDMOREPARAMS);
	}
	else
		chan.setKeyMask(false);
}

//RPL_CHANNELMODEIS a faire
void	Server::_modeChannel(std::string const chanName, std::string const mode, std::string const option, Client &client) {
	std::map<std::string, Channel>::iterator it;
	Channel chan;

	if (mode.size() && (!(mode[0] == '+') || !(mode[0] == '-')))
		clerr(ERR_UNKNOWNMODE);
	if (mode.size() > 2)
		clerr(ERR_UNKNOWNMODE);
	it = _channels.find(chanName);
	if (it == _channels.end())
		clerr(ERR_NOSUCHCHANNEL);
	chan = it->second;
	if (_isClientOp(chan, client)) {
		char c = mode[1];
		switch (c) {
			case 'o':
				_modeO(chan, mode, option);
				break;
			case 'p':
				chan.setPrivateMask(mode[0] == '+');
				break;
			case 's':
				chan.setSecretMask(mode[0] == '+');
				break;
			case 'i':
				chan.setInviteMask(mode[0] == '+');
				break;
			case 'm':
				chan.setModeratedMask(mode[0] == '+');
				break;
			case 'l':
				if (mode[0] == '+') {
					chan.setUserLimitMask(true);
					if (option.size()) {
						if (!chan.setUserLimit(option))
							clerr(ERR_NEEDMOREPARAMS);
					}
					else
						clerr(ERR_NEEDMOREPARAMS);
				}
				else
					chan.setUserLimitMask(false);
				break;
			case 'b':
				_modeB(chan, mode, option);
				break;
			case 'v':
				chan.setVoiceMask(mode[0] == '+');
				break;
			case 'k':
				
				break;
			default :
				clerr(ERR_UNKNOWNMODE);
		}
	}
}

void Server::mode(std::string const name, std::string const mode, std::string option, Client &client) {
	if (name[0] == '&' || name[0] == '#')
		_modeChannel(name, mode, option, client);
	else
		client.modeUser(name, mode);
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