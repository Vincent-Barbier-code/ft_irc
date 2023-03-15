#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# pragma once

#include <string>
#include <vector>
#include <map>
#include <list>
#include <stdexcept>

class Message {

    typedef void (Message::*func_type)(void);

    public:


        static std::vector<Message> parseAllMsg(std::string const & raw_msgs);
        static std::string _shrinkNBlank(std::string const & str, size_t n = std::string::npos);

        std::string const & getRaw() const;
        std::string const & getCmd() const;


        //~Message();



    private:
        
        static bool _isParsersInitialized;
        static std::map<std::string, func_type> _parsers;

        static void _initParsers();
        
        std::string _raw;
        std::string _cmd;
        std::vector<std::string> _params;


        Message(std::string const & raw_msg);

        void _initParams();
        void _parseUSER();

};


std::vector<std::string>    ke_split(std::string const & str, std::string const & pattern);
#endif