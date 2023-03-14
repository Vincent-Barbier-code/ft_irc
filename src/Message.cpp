#include "Message.hpp"

std::list<std::string>    ke_split(std::string const & str, std::string const & pattern) {
    std::list<std::string> lst;
    
    size_t index = 0;
    while (str[index]) {

        size_t end = str.find(pattern, index);
        std::string el = str.substr(index, end - index);
        index = end + 2;
        lst.push_back(el);
    }
    return lst;
}

std::list<Message> Message::parseAllMsg(std::string const & raw_msgs) {
    
    /*std::list<Message> msgs;
    
    size_t index = 0;
    while (raw_msgs[index]) {

        size_t end = raw_msgs.find("\r\n", index);
        std::string msg_str = raw_msgs.substr(index, end - index);
        index = end + 2;
        msgs.push_back(Message(msg_str));
    }
    return msgs;*/
    std::list<std::string> splited_msgs = ke_split(raw_msgs, "\r\n");
    std::list<Message> msgs;

    std::list<std::string>::const_iterator it;
    for (it = splited_msgs.begin(); it != splited_msgs.end(); it++)
        msgs.push_back(Message(*it));
    return msgs;
}

Message::Message(std::string const & raw_msg) {
    _raw = raw_msg;

    _cmd = _raw.substr(0, _raw.find(' '));
}


std::string const & Message::getCmd() const {
    return _cmd;
}

std::string const & Message::getRaw() const {
    return _raw;
}

