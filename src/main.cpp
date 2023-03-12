#include "ft_irc.hpp"
#include "server.hpp"

#define MAIN 0
#define TEST1 1

#if MAIN
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
	
	Client client1(11, serverAddress, "User1", "Nick1");
	Client client2(12, serverAddress, "User2", "Nick2");

	// Test de la fonction findClientByNickName
	Client* foundClient = client1.findClientByNickName("Nick1");
	if (foundClient != NULL) {
		std::cout << "Client found: " << foundClient->getUserName() << std::endl;
	} else {
		std::cout << "Client not found" << std::endl;
	}

	foundClient = client1.findClientByNickName("Nick2");
	if (foundClient != NULL) {
		std::cout << "Client found: " << foundClient->getUserName() << std::endl;
	} else {
		std::cout << "Client not found" << std::endl;
	}

	foundClient = client1.findClientByNickName("Nick3");
	if (foundClient != NULL) {
		std::cout << "Client found: " << foundClient->getUserName() << std::endl;
	} else {
		std::cout << "Client not found" << std::endl;
	}

	// Test de la fonction nick
	client1.nick("Nick2");
	client1.nick("Nick3");

	// Test de la fonction getNumericReplyMessage
	std::string reply = client1.getNumericReplyMessage(RPL_WELCOME);
	std::cout << "RPL_WELCOME reply: " << reply << std::endl;

	reply = client1.getNumericReplyMessage(ERR_ERRONEUSNICKNAME);
	std::cout << "ERR_ERRONEUSNICKNAME reply: " << reply << std::endl;

	reply = client1.getNumericReplyMessage(ERR_NICKNAMEINUSE);
	std::cout << "ERR_NICKNAMEINUSE reply: " << reply << std::endl;

	return 0;
}


#endif