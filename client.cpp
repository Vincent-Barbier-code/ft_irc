#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./client <server_address> <server_port>" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Récupération des arguments
    char* server_address = argv[1];
    int server_port = atoi(argv[2]);

    // Création du socket client
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Erreur: Impossible de créer le socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_address, &server_addr.sin_addr) <= 0) {
        std::cerr << "Erreur: Adresse IP invalide" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Erreur: Impossible de se connecter au serveur" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Envoi de données au serveur
    char buffer[BUFFER_SIZE];
    std::cout << "Entrez un message à envoyer au serveur: ";
    std::cin.getline(buffer, BUFFER_SIZE);
    send(sockfd, buffer, strlen(buffer), 0);

    // Réception de données du serveur
    memset(buffer, 0, BUFFER_SIZE);
    int nbBytes = recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (nbBytes < 0) {
        std::cerr << "Erreur: Impossible de recevoir des données du serveur" << std::endl;
        exit(EXIT_FAILURE);
    } else if (nbBytes == 0) {
        std::cout << "Le serveur s'est déconnecté" << std::endl;
    } else {
        std::cout << "Message reçu du serveur: " << buffer << std::endl;
    }

    // Fermeture du socket client
    close(sockfd);

    return 0;
}
