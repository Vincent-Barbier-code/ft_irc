#include "server.hpp"

//template <typename clients_ctn, typename fds_ctn>
std::list<Client *> Server::filterClientsByFd(std::map<int, Client *> const & clients, std::vector<int> const & fds) {
    std::list<Client *> filtered;

    for (std::vector<int>::const_iterator fd = fds.begin(); fd != fds.end(); ++fd)
        if (clients.find(*fd) != clients.end())
            filtered.push_back(clients.at(*fd));
    return (filtered);
}