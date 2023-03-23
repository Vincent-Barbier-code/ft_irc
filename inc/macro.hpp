#ifndef MACRO_HPP
#define MACRO_HPP
# pragma once
// default macro
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
// my macro
#define DEFAULT_PORT 8080
#define RPL_WELCOME 1

enum NICK_CODE {
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433
};

enum GENERAL_CODE {
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462
};

#endif
