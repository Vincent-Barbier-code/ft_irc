#include "Client.hpp"

//Commande : PASS
//Paramètres : <password>
void	Client::pass(std::string const &clientPass, std::string const &serverPass) {
	if (_isAuth)
		clerr(ERR_ALREADYREGISTRED);
	else if (clientPass == serverPass) {
		_isAuth = true;
		std::cout << "Client " << inet_ntoa(_addr.sin_addr) << " is now authentified" << std::endl;
	}
	else {
		std::cout << RED "Password mismatch" WHITE << std::endl;
		clerr(ERR_PASSWDMISMATCH);
	}
}