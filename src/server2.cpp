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
	std::string message = client._getNumericReplyMessage(code);
	std::cout << "Sending numeric reply to client ";
	
	_sendMsgToCLient(client, message);
}


void 		Server::_sendWelcomeMsg(Client const & client) {
	std::cout << "Welcoming: "; 
    _sendNumericReply(RPL_WELCOME, client);

}

void    Server::addChannel(Channel const & channel) {
    //exemple -> addChannel(Channel("#general", "Welcome to the general channel", client));
    std::cout << "adding channel: " << channel.getName() << std::endl;
    _channels[channel.getName()] = channel;

    std::cout << "Topic of new channel: " << _channels[channel.getName()].getTopic() << std::endl;
}