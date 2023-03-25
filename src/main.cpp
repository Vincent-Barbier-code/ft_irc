#include "ft_irc.hpp"
#include "server.hpp"

#define MAIN 1
#define TEST1 1

#if MAIN
int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: ./server <port> <password>" << std::endl;
		exit(EXIT_FAILURE);
	}


	//il faut proteger le atoi
	int port = atoi(argv[1]);

	Server server(port, argv[2]);
	signal(SIGQUIT, sigStop);

	//std::string s = "Je suis  la et toi t es ou !";

	//std::vector<std::string> vec = ke_split(s, " ");

	//for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); it++)
	//	std::cout << *it << std::endl;

	server.start();
    return 0;
}
#else

int main() {
	// Création de deux clients
	std::string ip = "127.0.0.1"; // adresse IP à utiliser
    int port = 8080; // numéro de port à utiliser

    // Créer un objet sockaddr_in
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // famille d'adresses
    serverAddress.sin_port = htons(port); // numéro de port (en ordre réseau)
    inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr);
	
	try
	{
	Client client1(11, serverAddress, "User1", "Nick1");
	Client client2(12, serverAddress, "User2", "Nick2");
	Client client3(12, serverAddress, "User5", "Nick3");
	
	// Test de la fonction findClientByNickName
	Client* foundClient = findClientByNickName("Nick1");
	if (foundClient != NULL) {
		std::cout << "Client found: " << foundClient->getUserName() << std::endl;
	} else {
		std::cout << "Client not found" << std::endl;
	}

	foundClient = findClientByNickName("Nick2");
	if (foundClient != NULL) {
		std::cout << "Client found: " << foundClient->getUserName() << std::endl;
	} else {
		std::cout << "Client not found" << std::endl;
	}

	foundClient = findClientByNickName("Nick3");
	if (foundClient != NULL) {
		std::cout << "Client found: " << foundClient->getUserName() << std::endl;
	} else {
		std::cout << "Client not found" << std::endl;
	}

	// Test de la fonction nick
	client1.nick("Nick2");
	std::cout << client1.getNickName() << std::endl;
	client1.nick("Nick3");
	std::cout << client1.getNickName() << std::endl;

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}


	return 0;
}


#endif