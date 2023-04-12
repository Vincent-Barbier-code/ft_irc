#include "server.hpp"
#include "color.hpp"

void Server::displayClients() {

    static char const *ANSI_COULEURS[] = {
        BLUE,
        PURPLE,
        GREEN
    };

    std::cout << ANSI_CLEAR_SCREEN ANSI_MOVE_CURSOR_HOME;

    int i = 0;
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        Client* client = it->second;
        std::cout << ANSI_COULEURS[i % 3] << client->getNickName() << "\001\033[0m\002" << std::endl;
    }
}