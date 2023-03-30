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


void Server::_join(int client_fd, std::vector<std::string> paramsV)
{
	std::map<std::string, Channel>::iterator it2;

	if (paramsV[0][0] != '#' || paramsV[0][0] != '&')
		throw Client::ClientException(ERR_NOSUCHCHANNEL);
	if (paramsV[0].size() > 50)
		throw Client::ClientException(ERR_NOSUCHCHANNEL);
	if (paramsV.size() == 0)
		throw Client::ClientException(ERR_NEEDMOREPARAMS);
	// if mode invitation only, check if client is invited to do
	if (_channels.at(paramsV[0]).getinviteMask() == 1)
		if (!_channels.at(paramsV[0]).isInInviteList(client_fd))
			throw Client::ClientException(ERR_INVITEONLYCHAN);
	// if not banned to do
	if (_channels.at(paramsV[0]).getbanMask() == 1)
		if (_channels.at(paramsV[0]).isInBanList(client_fd))
			throw Client::ClientException(ERR_BANNEDFROMCHAN);
	it2 = _channels.find(paramsV[0]);
	if (it2 == _channels.end())
	{
		if (_channels.at(paramsV[0]).getkeyMask() == 1)
		{
			if (paramsV[1] != _channels.at(paramsV[0]).getPassword())
				throw Client::ClientException(ERR_BADCHANNELKEY);
		}
		addChannel(Channel(paramsV[0], paramsV[1], *_clients.at(client_fd)));
	}
	// _channels.at(paramsV[0]).addClient(_clients.at(client_fd));
	_sendMsgToCLient(*_clients.at(client_fd), "JOIN " + paramsV[0]);
	_sendMsgToCLient(*_clients.at(client_fd), "RPL_TOPIC " + paramsV[0] + " " + _channels.at(paramsV[0]).getTopic());
	_sendMsgToCLient(*_clients.at(client_fd), "RPL_NAMREPLY " + paramsV[0] + " " + _channels.at(paramsV[0]).getUserList());
}