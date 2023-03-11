#include "Message.hpp"

std::list<Message> Message::parseAllMsg(std::string const & raw_msgs) {
    
    std::list<Message> msgs;
    
    size_t index = 0;
    while (raw_msgs[index]) {

        size_t end = raw_msgs.find("\r\n", index);
        std::string msg_str = raw_msgs.substr(index, end - index);
        index = end + 2;
        msgs.push_back(Message(msg_str));
    }
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

