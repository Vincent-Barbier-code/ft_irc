#include "ft_irc.hpp"
#include "server.hpp"

void	Server::_morse(int client_fd, std::string const & msg) {
	std::string morse;
	std::map<char, std::string> morseMap;

	morseMap['a'] = ".-";
	morseMap['b'] = "-...";
	morseMap['c'] = "-.-.";
	morseMap['d'] = "-..";
	morseMap['e'] = ".";
	morseMap['f'] = "..-.";
	morseMap['g'] = "--.";
	morseMap['h'] = "....";
	morseMap['i'] = "..";
	morseMap['j'] = ".---";
	morseMap['k'] = "-.-";
	morseMap['l'] = ".-..";
	morseMap['m'] = "--";
	morseMap['n'] = "-.";
	morseMap['o'] = "---";
	morseMap['p'] = ".--.";
	morseMap['q'] = "--.-";
	morseMap['r'] = ".-.";
	morseMap['s'] = "...";
	morseMap['t'] = "-";
	morseMap['u'] = "..-";
	morseMap['v'] = "...-";
	morseMap['w'] = ".--";
	morseMap['x'] = "-..-";
	morseMap['y'] = "-.--";
	morseMap['z'] = "--..";
	morseMap['0'] = "-----";
	morseMap['1'] = ".----";
	morseMap['2'] = "..---";
	morseMap['3'] = "...--";
	morseMap['4'] = "....-";
	morseMap['5'] = ".....";
	morseMap['6'] = "-....";
	morseMap['7'] = "--...";
	morseMap['8'] = "---..";
	morseMap['9'] = "----.";

	for (size_t i = 0; i < msg.size(); i++) {
		if (!isalpha(msg[i]) && !isdigit(msg[i]))
			clerr(ERR_NOTALPHANUMERIC);
		if (msg[i] == ' ')
			morse += " ";
		else {
			morse += morseMap[msg[i]];
			morse += " ";
		}
	}
	_sendMsgToClient(*_findClientByFd(client_fd), "PRIVMSG " + _clients[client_fd]->getNickName() + " :" + morse);
}
