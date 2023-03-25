#include "server.hpp"


void    Server::_sendData(int client_fd, std::string const & data) {

    std::cout << "send data to " << client_fd << std::endl;
    std::cout << "data: " << data << std::endl;
    if (send(client_fd, data.c_str(), data.size(), 0) == -1) {
        perror("send error");
        throw std::runtime_error("send error");
    }
}

void    Server::_sendData(Client const & client, std::string const & data) {
    _sendData(client.getFd(), data);
}

std::string const & Server::getServerName() {
    return _serverName;
}