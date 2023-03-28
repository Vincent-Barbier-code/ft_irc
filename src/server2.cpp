#include "server.hpp"

void    Server::_sendData(int client_fd, std::string const & data) {

    //std::cout << "send data to " << client_fd << std::endl;
    //std::cout << "data: " << data << std::endl;
    if (send(client_fd, data.c_str(), data.size(), 0) == -1) {
        perror("send error");
        throw std::runtime_error("send error");
    }
}

void    Server::_sendData(Client const & client, std::string const & data) {
    _sendData(client.getFd(), data);
}

void Server::_sendMsgToCLient(Client const & client, std::string const & msg) {
    std::string data = ":" + client.getServerName() + " " + msg + "\r\n";

    std::cout << BLUE << client.getNickName() << WHITE ": |" CYAN << data << WHITE "|" << std::endl; 
    _sendData(client.getFd(), data);
}

void	Server::_sendNumericReply(int code, Client const & client){
	std::string message = client._getNumericReplyMessage(code);
	std::cout << "Sending numeric reply to client ";
	
	_sendMsgToCLient(client, message);
}


void 		Server::_sendWelcomeMsg(Client const & client) {
    std::string message = client._getNumericReplyMessage(RPL_WELCOME);
	std::cout << "Sending Welcome message to client "; 
    
    _sendMsgToCLient(client, message);
}