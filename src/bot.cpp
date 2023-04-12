#include "client.hpp"

std::string Client::_lookForMorse(std::string msg) const { 
	std::string morseMsg = "";
	std::size_t	found;

	std::cout << msg << std::endl;
	if (!msg.size())
		return(morseMsg);
	found = msg.find("morse:");
	if (found != std::string::npos && found == 0) {
		morseMsg = msg;
		morseMsg.erase(found, 6);
	}
	std::cout << morseMsg << std::endl;
	return (morseMsg);
}

std::map<char, std::string>	_initMorseMap(void) {
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
	morseMap['A'] = ".-";
	morseMap['B'] = "-...";
	morseMap['C'] = "-.-.";
	morseMap['D'] = "-..";
	morseMap['E'] = ".";
	morseMap['F'] = "..-.";
	morseMap['G'] = "--.";
	morseMap['H'] = "....";
	morseMap['I'] = "..";
	morseMap['J'] = ".---";
	morseMap['K'] = "-.-";
	morseMap['L'] = ".-..";
	morseMap['M'] = "--";
	morseMap['N'] = "-.";
	morseMap['O'] = "---";
	morseMap['P'] = ".--.";
	morseMap['Q'] = "--.-";
	morseMap['R'] = ".-.";
	morseMap['S'] = "...";
	morseMap['T'] = "-";
	morseMap['U'] = "..-";
	morseMap['V'] = "...-";
	morseMap['W'] = ".--";
	morseMap['X'] = "-..-";
	morseMap['Y'] = "-.--";
	morseMap['Z'] = "--..";
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
	morseMap[' '] = " ";

	return (morseMap);
}

void	Client::_morse(Channel const &chan, std::string const & msg) const {
	std::string morse;
	std::map<char, std::string> morseMap;
	
	morseMap = _initMorseMap();
	for (size_t i = 0; i < msg.size(); i++) {
		if ((msg[i] != ' ') && !isalpha(msg[i]) && !isdigit(msg[i]))
			clerr(ERR_NOTALPHANUMERIC);
		if (msg[i] == ' ')
			morse += " ";
		else {
			morse += morseMap[msg[i]];
			morse += " ";
		}
	}
	botSendMsgToClient(*this, "PRIVMSG " + chan.getName() + " :" + morse);
}
