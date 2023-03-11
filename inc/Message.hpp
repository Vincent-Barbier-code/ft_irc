#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# pragma once

#include <string>
#include <list>

class Message {

    public:
        static std::list<Message> parseAllMsg(std::string const & raw_msgs);

        std::string const & getRaw() const;


        //~Message();



    private:
        
        std::string _raw;

        Message(std::string const & raw_msg);

};


#endif