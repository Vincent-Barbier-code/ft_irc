/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbarbier <vbarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:25:06 by vbarbier          #+#    #+#             */
/*   Updated: 2023/02/21 20:58:28 by vbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// cpp librairies
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

// c librairies
#include <cstdio>
#include <cstdlib>
#include <cstring>

// my librairies
#include "macro.hpp"
#include "socket.hpp"
