#ifndef FT_IRC_HPP
#define FT_IRC_HPP
# pragma once

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
#include <fstream>
#include <iostream>
#include <map>
#include <string>
// c librairies
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/epoll.h>

// my librairies
#include "macro.hpp"

void sigStop(int signum);

#endif
