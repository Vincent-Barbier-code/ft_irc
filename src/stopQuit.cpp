#include "Server.hpp"

bool	g_shutdown = false;
void Server::_partChannels()
{
	std::map<std::string, Channel>::iterator	it = _channels.begin();

	while (it != _channels.end())
	{
		std::cerr << "Parting channel " << it->first << std::endl;
		std::vector<int>	users = it->second.getUserList();
		for (size_t i = 0; i < users.size(); i++)
		{
			std::cerr << "Parting channel " << it->first << " for client " << i << std::endl;
			_part(users[i], it->first);
		}
		it++;
	}
}

void	sigStop(int signum)
{
	(void)signum;
	std::cerr << "Sigquit received." << std::endl;
	g_shutdown = 1;
}

void	Server::_eraseClient()
{
	std::map<int, ClientPtr>::iterator it = _clients.begin();
	std::map<int, ClientPtr>::iterator ite = _clients.end();

	while(it != ite)
	{
		close(it->first); //first is used to access the key (fd of client)
		delete it->second; //second is used to access the value (pointer to client)
		_clients.erase(it++);
	}
}

void Server::_eraseChannel()
{
	std::map<std::string, Channel>::iterator it = _channels.begin();
	std::map<std::string, Channel>::iterator ite = _channels.end();

	while(it != ite)
		_channels.erase(it++);
}

void Server::_freeAndClose()
{
	//_partChannels();
	_eraseClient();
	_eraseChannel();
	close(_epoll_fd);
	close(_server_fd);
}

void	Server::stop()
{
	std::cerr << "Server is shutting down..." << std::endl;
	_freeAndClose();
	exit(EXIT_SUCCESS);
}