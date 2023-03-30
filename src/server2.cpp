#include "server.hpp"

void    Server::sendData(Client const & client, std::string const & data) {

    //std::cout << "send data to " << client_fd << std::endl;
    //std::cout << "data: " << data << std::endl;
    if (send(client.getFd(), data.c_str(), data.size(), 0) == -1) {
        perror("send error");
        throw std::runtime_error("send error");
    }
}


void Server::_sendMsgToCLient(Client const & client, std::string const & msg) {
    std::string data = ":" + client.getServerName() + " " + msg + "\r\n";

    std::cout << BLUE << client.getNickName() << WHITE ": |" CYAN << data << WHITE "|" << std::endl; 
    sendData(client, data);
}

void	Server::_sendNumericReply(int code, Client const & client){
	std::string message = _getNumericReplyMessage(code, client);
	std::cout << "Sending message to client " << client.getNickName() << ": " << message << std::endl;

	_sendMsgToCLient(client, message);
}


void 		Server::_sendWelcomeMsg(Client const & client) {
	std::cout << "Welcoming: "; 
    _sendNumericReply(RPL_WELCOME, client);

}

void    Server::addChannel(Channel const & channel) {
    //exemple -> addChannel(Channel("#general", "Welcome to the general channel", client));
    std::cout << "adding channel: " << channel.getName() << std::endl;
    if (_channels.find(channel.getName()) != _channels.end()) {
        std::cout << "Channel already exists" << std::endl;
        throw std::runtime_error("addChannel() -> Channel already exists");
    }
    _channels[channel.getName()] = channel;

    std::cout << "Topic of new channel: " << _channels[channel.getName()].getTopic() << std::endl;
}


void Server::_parseJoin(int client_fd, std::string const & name)
{
	if (name.size() == 0)
		clerr(ERR_NEEDMOREPARAMS);
	if (name[0] != '#' || name[0] != '&')
		clerr(ERR_NOSUCHCHANNEL);
	if (name.size() > 200)
		clerr(ERR_NOSUCHCHANNEL);
	if (name.find(',') != std::string::npos || name.find(' ') != std::string::npos)
		clerr(ERR_NOSUCHCHANNEL);
	// if mode invitation only, check if client is invited to do
	if (_channels.at(name).getinviteMask() == 1)
		if (!_channels.at(name).isInInviteList(client_fd))
			clerr(ERR_INVITEONLYCHAN);
	// if not banned to do
	if (_channels.at(name).getbanMask() == 1)
		if (_channels.at(name).isInBanList(client_fd))
			clerr(ERR_BANNEDFROMCHAN);

}


void Server::_join(int client_fd, std::string const & name, std::string const & key)
{
	std::map<std::string, Channel>::iterator it2;

	_parseJoin(client_fd, name);
	std::cout << "parserJOIN" << std::endl;
	it2 = _channels.find(name);
	if (it2 == _channels.end())
	{
		if (_channels.at(name).getkeyMask() == 1)
		{
			if (key != _channels.at(name).getPassword())
				clerr(ERR_BADCHANNELKEY);
		}
		addChannel(Channel(name, key, *_clients.at(client_fd)));
		std::cout << "A2" << std::endl;

	}
	std::cout << "Adding client " << _clients.at(client_fd)->getNickName() << " to channel " << name << std::endl;
	// _channels.at(name).addClient(_clients.at(client_fd));
	_sendMsgToCLient(*_clients.at(client_fd), "JOIN " + name);
	_sendMsgToCLient(*_clients.at(client_fd), itostr(RPL_TOPIC) + " " + _clients.at(client_fd)->getNickName() + " " + name + " :" + _channels.at(name).getTopic());
	_sendMsgToCLient(*_clients.at(client_fd), itostr(RPL_NAMREPLY) + " " + _clients.at(client_fd)->getNickName() + " " + name + " " + _channels.at(name).getUserList());
}