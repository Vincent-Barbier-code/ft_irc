#include "ft_irc.hpp"
#include "server.hpp"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: ./server <port>" << std::endl;
		exit(EXIT_FAILURE);
	}

	int port = atoi(argv[1]);

	Server server(port);
	server.start();
    return 0;
}
