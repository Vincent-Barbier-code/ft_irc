#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# pragma once

#include <string>
#include <list>

class Message {

    public:
        static std::list<Message> parseAllMsg(std::string const & raw_msgs);

        std::string const & getRaw() const;
        std::string const & getCmd() const;


        //~Message();



    private:
        
        std::string _raw;
        std::string _cmd;

        Message(std::string const & raw_msg);

};


std::list<std::string>    ke_split(std::string const & str, std::string const & pattern);
#endif