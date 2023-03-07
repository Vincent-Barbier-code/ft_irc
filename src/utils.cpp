#include "ft_irc.hpp"
#include <sstream>

// Converti un int en string
// std::string itostr(int i) {
// 	std::string s;

// 	while (i > 0) {
// 		s = (char)(i % 10 + '0') + s;
// 		i /= 10;
// 	}
// 	return (s);
// }

std::string itostr(int i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}