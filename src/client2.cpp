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

std::string Client::getHostName() const {
    return _hostName;
}

std::string Client::getServerName() const {
    return _serverName;
}

std::string Client::getRealName() const {
    return _realName;
}



void Client::_sendMsgToCLient(Client const & client, std::string const & msg) const {
    std::string data = ":" + client.getNickName() + " " + msg + "\r\n";

    std::cout << BLUE << client.getNickName() << WHITE ": |" CYAN << data << WHITE "|" << std::endl; 
    Server::sendData(client, data);
}

void Client::sendPrivateMsg(Client const & receiver, std::string const & msg) const {
    
    std::string data = "PRIVMSG " + receiver.getNickName() + " :" + msg;
    _sendMsgToCLient(receiver, msg); 
}

void Client::sendPrivateMsg(Channel const & channel, std::string const & msg, client_map const & clients) const {
    std::vector<int> channelFds = channel.getUserList();
    std::vector<int> channelOps = channel.getOperatorList();
    std::list<Client *> channelClients = Server::filterClientsByFd(clients, channelFds);

    if (find(channelFds.begin(), channelFds.end(), _fd) == channelFds.end())
        clerr(ERR_CANNOTSENDTOCHAN);
    if (channel.isModerated() && find(channelOps.begin(), channelOps.end(), _fd) == channelOps.end())
        clerr(ERR_CANNOTSENDTOCHAN);
        
    for (std::list<Client*>::const_iterator client = channelClients.begin(); client != channelClients.end(); client++) {
        std::string data = "PRIVMSG " + channel.getName() + " :" + msg;
        _sendMsgToCLient(**client, data);
    }
}