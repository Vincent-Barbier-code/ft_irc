#include "Server.hpp"

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

std::list<Client *> Server::filterClientsByFd(std::map<int, Client *> const & clients, std::vector<int> const & fds) {
    std::list<Client *> filtered;

    for (std::vector<int>::const_iterator fd = fds.begin(); fd != fds.end(); ++fd)
        if (clients.find(*fd) != clients.end())
            filtered.push_back(clients.at(*fd));
    return (filtered);
}

bool	Server::_isClientOp(Channel const & chan, Client &client) {
	if (!chan.isClientInList(chan.getUserList(), client.getFd()))
		clerr(ERR_NOTONCHANNEL);
	if (!chan.isClientInList(chan.getOpList(), client.getFd()))
		clerr(ERR_CHANOPRIVSNEEDED);
	return (true);
}