#ifndef MACRO_HPP
#define MACRO_HPP
# pragma once

#define clerr(code) throw Client::ClientException(code)

// default macro
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
// my macro
#define MAX_NICKNAME_LENGTH 9
#define MAX_USERNAME_LENGTH 9
#define SERVER_NAME "ft_irc"

#define DEFAULT_PORT 8080
#define RPL_WELCOME 1
#define RPL_YOURHOST 2
#define RPL_CREATED 3
#define RPL_MYINFO 4
#define RPL_LUSERCLIENT 251
#define RPL_LUSEROP 252
#define RPL_LUSERCHANNELS 254
#define RPL_MOTD 372
#define RPL_MOTDSTART 375
#define RPL_ENDOFMOTD 376
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTRED 462
#define ERR_PASSWDMISMATCH 464


#endif
