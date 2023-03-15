#include "Message.hpp"

std::vector<std::string>    ke_split(std::string const & str, std::string const & pattern) {
    std::vector<std::string> vec;
    
    size_t index = 0;
    while (str[index]) {

        size_t end = str.find(pattern, index);
        std::string el = str.substr(index, end - index);
        index = end + 2;
        vec.push_back(el);
    }
    return vec;
}

bool Message::_isParsersInitialized = false;
std::map<std::string, Message::func_type> Message::_parsers;

void Message::_initParsers(void) {
    if (_isParsersInitialized)
        return ;
    _parsers["USER"] = &Message::_parseUSER;
}

std::string Message::_shrinkNBlank(std::string const & str, size_t n /* = std::string::npos*/) {

    std::string s;
    size_t sp_i = 0;
    size_t ch_i   = 0;

    if (str.at(0) == ' ' || str.at(str.length() - 1) == ' ')
        throw std::invalid_argument("Leading or trailing blank space");
    
    while (n--) {
        sp_i = str.find(' ', ch_i);
        if (sp_i == std::string::npos) break;
            //return s += str.substr(ch_i, sp_i);
        s += str.substr(ch_i, sp_i - ch_i + 1);
        ch_i = str.find_first_not_of(' ', sp_i);
    }
    s += str.substr(ch_i, std::string::npos);

    return s;
    
}

void Message::_parseUSER(void) {

    std::vector<std::string> space_splited = ke_split(_raw, " ");

    std::list<std::string> params;
    copy(space_splited.begin(), space_splited.end(), back_inserter(params));

    params.pop_front();

    return ;
}

std::vector<Message> Message::parseAllMsg(std::string const & raw_msgs) {
    
    /*std::list<Message> msgs;
    
    size_t index = 0;
    while (raw_msgs[index]) {

        size_t end = raw_msgs.find("\r\n", index);
        std::string msg_str = raw_msgs.substr(index, end - index);
        index = end + 2;
        msgs.push_back(Message(msg_str));
    }
    return msgs;*/
    std::vector<std::string> splited_msgs = ke_split(raw_msgs, "\r\n");
    std::vector<Message> msgs;

    std::vector<std::string>::const_iterator it;
    for (it = splited_msgs.begin(); it != splited_msgs.end(); it++)
        msgs.push_back(Message(*it));
    return msgs;
}

Message::Message(std::string const & raw_msg) {

    _initParsers();

    // - - - - - -

    _raw = raw_msg;
    _cmd = _raw.substr(0, _raw.find(' '));
    
    //_initParams();
}


void Message::_initParams() {

    if (_cmd != "USER")
        return ;
    (this->*_parsers[_cmd])();
    
}

std::string const & Message::getCmd() const {
    return _cmd;
}

std::string const & Message::getRaw() const {
    return _raw;
}

