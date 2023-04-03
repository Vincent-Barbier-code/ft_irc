#include "server.hpp"

//template <typename clients_ctn, typename fds_ctn>
std::list<Client *> Server::filterClientsByFd(std::map<int, Client *> const & clients, std::vector<int> const & fds) {
    std::list<Client *> filtered;

    for (std::vector<int>::const_iterator fd = fds.begin(); fd != fds.end(); ++fd)
        if (clients.find(*fd) != clients.end())
            filtered.push_back(clients.at(*fd));
    return (filtered);
}

void Server::_sendMsgNumericToCLient(Client const & client, int code, std::string const & msg) {

    std::string data = itostr(code) + " " + client.getNickName() + " " + msg;
    _sendMsgToCLient(client, data);
}

void Server::_user(Client & client, std::vector<std::string> const & params) {
    client.user(params[0], params[1], params[2], params[3]);
    _sendWelcomeMsg(client);
}