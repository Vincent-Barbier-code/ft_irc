#include "client.hpp"
#include "utils.hpp"


void Client::user(std::string const & username, std::string const & hostname,
                  std::string const & servername, std::string realname) {

 
    if (realname[0] != ':')
        throw std::invalid_argument("USER: Invalid realname no ':' at the beginning");
    realname.erase(0, 1);
    if (!isValid(realname))
        throw std::invalid_argument("USER: Invalid realname");
    else if (realname.size() > 50)
        throw std::invalid_argument("USER: Realname too long");
    else if (!isValid(username))
        throw std::invalid_argument("USER: Invalid username");
    else if (_isRegistered)
        throw std::invalid_argument("USER: User already registered");
    else if (_nickName.empty())
        throw std::invalid_argument("USER: Nickname not set");
    
    _userName   = username;
    _hostName   = hostname;
    _serverName = servername;
    _realName   = realname;
    _isRegistered = true;

    std::cout << "COMMAND USER executed: " << _userName << " " << _hostName << " " << _serverName << " " << _realName << std::endl;
    std::cout << "nickname: " << _nickName << std::endl; 
}

std::string Client::getHostName() const{
    return _hostName;
}

std::string Client::getServerName() const {
    return _serverName;
}

std::string Client::getRealName() const{
    return _realName;
}