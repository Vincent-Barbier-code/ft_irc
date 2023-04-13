#include "Server.hpp"

void Server::_quit(Client & client, std::string const & msg) {
    std::string quitMsg = "QUIT :" + (msg == "" ? "for no reason" : msg);
    client.sendMsgToStalkers(quitMsg, _channels, _clients);
    _deconnection(client.getFd());
}