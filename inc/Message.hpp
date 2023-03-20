#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# pragma once

#include <string>
#include <vector>
#include <map>
#include <list>
#include <stdexcept>

#include "utils.hpp"

class Param
{
    private:
        std::string _name;
        std::string _value;
    public:
        Param(std::string const &name, std::string const &value);
        Param(Param const &src);
        Param &operator=(Param const &rhs);

        std::string const & getName() const;
        std::string const & getValue() const;
};

class Message {

    typedef void (Message::*func_type)(void);

    public:

        static std::vector<Message> parseAllMsg(std::string const & raw_msgs);

        std::string const & getRaw() const;
        std::string const & getCmd() const;

    private:
        
        static bool _isParsersInitialized;
        static std::map<std::string, func_type> _parsers;

        static void _initParsers();
        
        std::string _raw;
        std::string _cmd;
        std::vector<Param> _params;

        Message(std::string const & raw_msg);

        void _initParams();
        void _parseCAP();
        void _parseNICK();
        void _parseUSER();

};

#endif