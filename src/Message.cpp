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
    _parsers["PASS"] = &Message::_parsePASS;
    //_parsers["CAP"] = &Message::_parseCAP;
    _parsers["USER"] = &Message::_parseUSER;
    _parsers["NICK"] = &Message::_parseNICK;
    _parsers["JOIN"] = &Message::_parseJOIN;
    _parsers["QUIT"] = &Message::_parseQUIT;
    _parsers["LIST"] = &Message::_parseLIST;
    _parsers["PRIVMSG"] = &Message::_parsePRIVMSG;
    _parsers["KICK"] = &Message::_parseKICK;
    _parsers["INVITE"] = &Message::_parseINVITE;
    _parsers["PING"] = &Message::_parsePING;
    _parsers["MODE"] = &Message::_parseMODE;
    _parsers["PART"] = &Message::_parsePART;
    _parsers["TOPIC"] = &Message::_parseTOPIC;
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
    _rawWPrefix = raw_msg;
    _raw = _rawWPrefix[0] == ':' ? _rawWPrefix.substr(_rawWPrefix.find(' ') + 1) : _rawWPrefix;
    _prefix = _rawWPrefix[0] == ':' ? _rawWPrefix.substr(1, _rawWPrefix.find(' ') - 1) : "";
    _cmd = _raw.substr(0, _raw.find(' '));
    _err = 0;
    _initParams();
}


void Message::_initParams() {

    if (_parsers.find(_cmd) == _parsers.end())
        _err = INVALID_CMD;
    else
        (this->*_parsers[_cmd])();
    
}

// --------------------- operator -------------------------


std::ostream & operator<<(std::ostream & o, Message const & msg) {
    o   << WHITE "RAW_MSG: " PURPLE << std::setw(50) << msg.getRawWPrefix()
        << WHITE "  |   CMD: " PURPLE << std::setw(5)  << msg.getCmd() << WHITE
        << "  |   PARAMS: " PURPLE << msg.paramsToString() << WHITE;

    return o;
}



// ----------------------- Getters ----------------------------

std::string const & Message::getRawWPrefix() const {
    return _rawWPrefix;
}

std::string const & Message::getPrefix() const {
    return _prefix;
}

std::string const & Message::getCmd() const {
    return _cmd;
}

std::string const & Message::getRaw() const {
    return _raw;
}

std::vector<Param> const & Message::getParams() const {
    return _params;
}

std::vector<std::string> Message::getParamsValues() const {
    std::vector<std::string> values;
    for (size_t i = 0; i < _params.size(); i++)
        values.push_back(_params[i].getValue());
    return values;
}

int Message::getErr(void) const {
    return _err;
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

void Message::_parsePASS(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");
    _params.push_back(Param("password", space_splited[1]));
}

void Message::_parseCAP(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");
    _params.push_back(Param("idk", space_splited[1]));
}

void Message::_parseNICK(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");
    if (space_splited.size() != 2)
        _err = ERR_NEEDMOREPARAMS;
    else
        _params.push_back(Param("nickname", space_splited[1]));
}

void Message::_parseUSER(void) {

    std::vector<std::string> space_splited = ke_split(_raw, " ");
    
    std::string realname = space_splited[4];

    for (size_t i = 5; i < space_splited.size(); i++) {
        realname += " " + space_splited[i];
    }
    

    _params.push_back(Param("username", space_splited[1]));
    _params.push_back(Param("hostname", space_splited[2]));
    _params.push_back(Param("servername", space_splited[3]));
    _params.push_back(Param("realname", realname));

}

void Message::_parseQUIT(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");
    if (space_splited.size() == 1)
        return ; // no message after QUIT command
    if (space_splited[1][0] != ':') {
        std::cout << "Invalid QUIT message" << std::endl;
        _err = ERR_NEEDMOREPARAMS;
        return ;
    }
    std::string message = space_splited[1].substr(1);
    for (size_t i = 2; i < space_splited.size(); i++) {
        message += " " + space_splited[i];
    }
    _params.push_back(Param("message", message));
}

void Message::_parseLIST(void) {
    //Cette fonction ne fait rien, oui, elle est vide, mais elle est nécessaire pour que le parser fonctionne
    //car elle est appelée dans _initParams()
    //Si vous avez une meilleure idée, je suis preneur
    //Sinon, je vous laisse avec cette fonction vide
    //Merci
    //Bonne journée
    //A bientôt
    //Bisous
    //Je t'aime
}

void Message::_parsePRIVMSG(void) {
    std::vector<std::string> space_splited = ke_nSplit(_raw, " ", 3);

    if (space_splited.size() == 1) {
        _err = ERR_NEEDMOREPARAMS;
        std::cerr << RED "Invalid PRIVMSG NO PARAMS" WHITE << std::endl;
    }
    if (space_splited.size() == 2) {
        _err = space_splited[1][0] == ':' ? ERR_NORECIPIENT : ERR_NOTEXTTOSEND;
        if (_err == ERR_NORECIPIENT)
            std::cerr << RED "Invalid PRIVMSG NO DESTINATION" WHITE << std::endl;
        else
            std::cerr << RED "Invalid PRIVMSG NO TEXTTOSEND" WHITE << std::endl;
    }

    if (_err) return;

    _params.push_back(Param("destinations", space_splited[1]));
    _params.push_back(Param("message", space_splited[2].substr(1)));
}

void Message::_parseINVITE(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");

    if (space_splited.size() == 1) {
        _err = ERR_NEEDMOREPARAMS;
        std::cerr << RED "Invalid INVITE NO PARAMS" WHITE << std::endl;
    }
    if (space_splited.size() == 2) {
        _err = ERR_NEEDMOREPARAMS;
        std::cerr << RED "Invalid INVITE NO CANAL" WHITE << std::endl;
    }

    if (_err) return;

    _params.push_back(Param("nickname", space_splited[1]));
    _params.push_back(Param("channel", space_splited[2]));
}

void Message::_parseKICK(void) {
    std::vector<std::string> space_splited = ke_nSplit(_raw, " ", 4);

    if (space_splited.size() < 3) {

        std::string strerr = "Invalid KICK ";
        _err               = ERR_NEEDMOREPARAMS;
        strerr += space_splited.size() == 1 ? "NO PARAMS" : "NO USER";
        return;
    }

    _params.push_back(Param("channel", space_splited[1]));
    _params.push_back(Param("nickname", space_splited[2]));
    if (space_splited.size() == 4)
        _params.push_back(Param("comment", space_splited[3].substr(1)));
}

void Message::_parseJOIN(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");

    if (space_splited.size() > 1)
        _params.push_back(Param("channel", space_splited[1]));
    
    if (space_splited.size() > 2)
        _params.push_back(Param("key", space_splited[2]));
}

void Message::_parsePING(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");
    if (space_splited.size() == 1)
        throw std::runtime_error("PARSE PING ARGUMENT PAS SUFFISANT") ; // a remplacer par le setting de la variable d'erreur
    _params.push_back(Param("server", space_splited[1]));
}

void Message::_parseMODE(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");

    if (space_splited.size() < 2) {
        std::cerr << RED "Invalid MODE" WHITE << std::endl;
        _err = ERR_NEEDMOREPARAMS;
        return;
    }

    _params.push_back(Param("channel", space_splited[1]));
    if (space_splited.size() >=  3)
        _params.push_back(Param("mode", space_splited[2]));
    if (space_splited.size() ==  4 )
        _params.push_back(Param("limit/user", space_splited[3]));
}

void Message::_parsePART(void) {
    std::vector<std::string> space_splited = ke_split(_raw, " ");

    if (space_splited.size() < 2) {
        std::cerr << RED "Invalid PART" WHITE << std::endl;
        _err = ERR_NEEDMOREPARAMS;
        return;
    }

    _params.push_back(Param("channel", space_splited[1]));
}

void Message::_parseTOPIC(void) {
    std::vector<std::string> space_splited = ke_nSplit(_raw, " ", 3);

    if (space_splited.size() < 2) {
        std::cerr << RED "Invalid TOPIC" WHITE << std::endl;
        _err = ERR_NEEDMOREPARAMS;
        return;
    }

    _params.push_back(Param("channel", space_splited[1]));
    _params.push_back(Param("topic", space_splited.size() == 3 ? space_splited[2].substr(1) : ""));
}
