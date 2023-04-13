#include "server.hpp"
#include "color.hpp"

void Server::displayClients() {
    
    if (DEBUG)
        return;

    const int largeur_colonne = 20;

	std::vector<std::string> couleurs;

    couleurs.push_back("\033[1;31m");
    couleurs.push_back("\033[1;32m");
    couleurs.push_back("\033[1;33m");
    couleurs.push_back("\033[1;34m");
    couleurs.push_back("\033[1;35m");
    couleurs.push_back("\033[1;36m");
    const std::string reset = "\033[0m";

    std::vector<std::string> titres;
    titres.push_back("Nickname");
    titres.push_back("Username");
    titres.push_back("Host");
    titres.push_back("RealName");
    titres.push_back("fd");

    std::vector<std::vector<std::string> > donnees;

    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        Client* client = it->second;
        std::vector<std::string> donnee;
        donnee.push_back(client->getNickName());
        donnee.push_back(client->getUserName());
        donnee.push_back(client->getHostName());
        donnee.push_back(client->getRealName());
        donnee.push_back(itostr(client->getFd()));
        donnees.push_back(donnee);
    }


    std::cout << ANSI_CLEAR_SCREEN << ANSI_MOVE_CURSOR_HOME;
    std::cout << "\t\t\t\t\t FT_IRC - CLIENTS CONNECTED\n\n";
    // Affichage des titres
 	std::cout << couleurs[0];

    for (std::size_t i = 0; i < titres.size(); ++i) {
        std::cout << "|" << std::setw(largeur_colonne) << std::left << titres[i].substr(0, largeur_colonne);
    }
    std::cout << "|" << reset << "\n";

    // Affichage de la ligne de séparation
	std::cout << couleurs[1];
	for (std::size_t i = 0; i < titres.size(); ++i) {
        std::cout << "+";
        for (int j = 0; j < largeur_colonne; ++j) std::cout << "-";
    }
    std::cout << "+" << reset << "\n";

    // Affichage des données
    for (std::size_t k = 0; k < donnees.size(); ++k) {
        std::cout << couleurs[k+2];
        for (std::size_t i = 0; i < donnees[k].size(); ++i) {
            std::string valeur_affichee;
            if (donnees[k][i].length() > largeur_colonne-3) {
                valeur_affichee = donnees[k][i].substr(0, largeur_colonne-3) + "...";
            } else {
                valeur_affichee = donnees[k][i];
            }
			// if (i  > 0)
			// 	std::cout << " ";
            std::cout << "|" << std::setw(largeur_colonne) << std::left << valeur_affichee.substr(0, largeur_colonne);
        }
        std::cout << "|" << reset << "\n";
    }

    // Affichage de la ligne de séparation

    std::cout << couleurs[1];
    for (std::size_t i = 0; i < titres.size(); ++i) {
        std::cout << "+";
        for (int j = 0; j < largeur_colonne; ++j) std::cout << "-";
    }
    std::cout << "+" << "\n" << reset;
}
