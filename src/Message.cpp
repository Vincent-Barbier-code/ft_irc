#include "Message.hpp"


// ------------------------ Param Class ----------------------------

Param::Param(std::string const &name, std::string const &value) : _name(name), _value(value) {}

Param::Param(Param const &src) : _name(src.getName()), _value(src.getValue()) {}

Param &Param::operator=(Param const &rhs) {
    _name = rhs.getName();
    _value = rhs.getValue();
    return *this;
}

std::string const & Param::getName() const { return _name; }
std::string const & Param::getValue() const { return _value; }


// ----------------- Message Class - non member static --------------

bool Message::_isParsersInitialized = false;
std::map<std::string, Message::func_type> Message::_parsers;

void Message::_initParsers(void) {
    if (_isParsersInitialized)
        return ;
    _parsers["CAP"] = &Message::_parseCAP;
    _parsers["USER"] = &Message::_parseUSER;
    _parsers["NICK"] = &Message::_parseNICK;
}

std::vector<Message> Message::parseAllMsg(std::string const & raw_msgs) {
    
    std::vector<std::string> splited_msgs = ke_split(raw_msgs, "\r\n");
    std::vector<Message> msgs;

    std::vector<std::string>::const_iterator it;
    for (it = splited_msgs.begin(); it != splited_msgs.end(); it++)
        msgs.push_back(Message(*it));
    return msgs;
}


// ----------------------- Contructors - Init --------------------

Message::Message(std::string const & raw_msg) {

    if (!_isParsersInitialized)
        _initParsers(); // static function to init the map of parsers

    // - - - - - -

    _raw = raw_msg;
    _cmd = _raw.substr(0, _raw.find(' '));
    _initParams();
}


void Message::_initParams() {

    if (_parsers.find(_cmd) == _parsers.end()) 
        throw std::invalid_argument("Invalid command");
    (this->*_parsers[_cmd])();
    
}

// --------------------- operator -------------------------


std::ostream & operator<<(std::ostream & o, Message const & msg) {
    o   << WHITE "RAW_MSG: " PURPLE << std::setw(50) << msg.getRaw()
        << WHITE "  |   CMD: " PURPLE << std::setw(5)  << msg.getCmd() << WHITE
        << "  |   PARAMS: "PURPLE << msg.paramsToString() << WHITE  << std::endl; 

    return o;
}



// ----------------------- Getters ----------------------------

std::string const & Message::getCmd() const {
    return _cmd;
}

std::string const & Message::getRaw() const {
    return _raw;
}


// --------------------- Members other --------------------------

std::string Message::paramsToString() const {

    std::string s;

    for (size_t i = 0; i < _params.size(); i++) {
        s += _params[i].getName();
        s += ": ";
        s += _params[i].getValue();
        if (i < _params.size() - 1)
            s += ", ";
    }

    return s;
}


// ---------------------- Parsers -----------------------

void Message::_parseCAP(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");
    _params.push_back(Param("idk", space_splited[1]));
}

void Message::_parseNICK(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");
    _params.push_back(Param("nickname", space_splited[1]));
}

void Message::_parseUSER(void) {

    std::vector<std::string> space_splited = ke_split(_raw, " ");
    
    std::string realname = space_splited[4];

    for (size_t i = 5; i < space_splited.size(); i++) {
        realname += " " + space_splited[i];
    }
    
    if (realname[0] != ':')
        throw std::invalid_argument("Invalid realname no ':' at the beginning");
    realname.erase(0, 1);

    _params.push_back(Param("username", space_splited[1]));
    _params.push_back(Param("hostname", space_splited[2]));
    _params.push_back(Param("servername", space_splited[3]));
    _params.push_back(Param("realname", realname));

}