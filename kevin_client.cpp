#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#define PORT 8085

int main(void) {




    int         client_fd;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if ((server_addr.sin_addr.s_addr = inet_addr("0.0.0.0")) == INADDR_NONE) {
		std::cerr << "inet_addr() invalid address" << std::endl;
		exit(EXIT_FAILURE);
	}

	if ((connect(client_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr))) == -1) {
		perror("connect failed");
		exit(EXIT_FAILURE);
	}

    char msg[] = "Salut je suis un msg du client";

	if (send(client_fd, msg, strlen(msg) + 1, 0) == -1) {
		perror("send failed");
		exit(EXIT_FAILURE);
	}

	char buf[1024];

	std::cout << "buf size: " << sizeof(buf) << std::endl;
	int len;
	if ((len = recv(client_fd, buf, sizeof(buf), 0)) == -1) {
		perror("recv failed");
		exit(EXIT_FAILURE);
	}

	//buf[len] = '\0';

	std::cout << "recv done !" << std::endl;
	std::cout << "Len: " << len << ", Message du serveur: " << buf << std::endl;

	


    return 0;
}