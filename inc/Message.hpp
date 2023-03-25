#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# pragma once

#include <string>
#include <vector>
#include <map>
#include <list>
#include <stdexcept>
#include <iostream>
#include <iomanip>

#include "utils.hpp"
#include "color.hpp"

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

    friend std::ostream & operator<<(std::ostream & o, Message const & msg);

    typedef void (Message::*func_type)(void);

    public:

        static std::vector<Message> parseAllMsg(std::string const & raw_msgs);

        std::string const & getRawWPrefix() const;
        std::string const & getRaw() const;
        std::string const & getCmd() const;
        std::string const & getPrefix() const;
        std::vector<Param> const & getParams() const;
        std::vector<std::string>   getParamsValues() const;
        
        std::string paramsToString() const;

    private:
        
        static bool _isParsersInitialized;
        static std::map<std::string, func_type> _parsers;

        static void _initParsers();
        
        std::string _rawWPrefix; // raw message with prefix
        std::string _raw; // raw message without prefix
        std::string _cmd; // command
        std::string _prefix; // prefix
        std::vector<Param> _params; // command params
        

        Message(std::string const & raw_msg);


        void _initParams();
        void _parseCAP();
        void _parseNICK();
        void _parseUSER();

};

std::ostream & operator<<(std::ostream & o, Message const & msg);

#endif