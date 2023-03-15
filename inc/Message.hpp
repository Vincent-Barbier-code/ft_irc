#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# pragma once

#include <string>
#include <vector>


class Message {

    enum CMD {
        NICK = 0,
        USER,
        NB_CMD = 2
    };

    public:


        static std::vector<Message> parseAllMsg(std::string const & raw_msgs);

        std::string const & getRaw() const;
        std::string const & getCmd() const;


        //~Message();



    private:
        
        static bool _isParserInitialized;
        static void (Message::* _parse[2])(void);

        void _initParser();
        
        std::string _raw;
        std::string _cmd;
        std::vector<std::string> _params;


        Message(std::string const & raw_msg);

        void _initParams();
        void _parseUSER();

};


std::vector<std::string>    ke_split(std::string const & str, std::string const & pattern);
#endif