#include "ft_irc.hpp"
#include "server.hpp"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: ./server <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (!checkAtoi(argv[1])) {
		std::cerr << "Port must be an int" << std::endl;
		exit(EXIT_FAILURE);
	}
	int port = atoi(argv[1]);

	Server server(port, argv[2]);
	signal(SIGQUIT, sigStop);
	server.start();
    return 0;
}
