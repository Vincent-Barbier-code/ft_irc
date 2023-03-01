/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 18:18:37 by vbarbier          #+#    #+#             */
/*   Updated: 2023/03/01 19:40:27 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// void cou(void) {
// 	std::stringstream ss;

// 	ss << 42;
// 	ss << 'c';
// 	ss << "de";
// 	std::cout << ss.str();
// }