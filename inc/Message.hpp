#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# pragma once

#include <string>
#include <vector>

class Message {

    public:
        static std::vector<Message> parseAllMsg(std::string const & raw_msgs);

        std::string const & getRaw() const;
        std::string const & getCmd() const;


        //~Message();



    private:
        
        std::string _raw;
        std::string _cmd;
        std::string _params;


        Message(std::string const & raw_msg);

};


std::vector<std::string>    ke_split(std::string const & str, std::string const & pattern);
#endif