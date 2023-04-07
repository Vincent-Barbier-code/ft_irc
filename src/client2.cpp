#include "client.hpp"
#include "utils.hpp"


void Client::user(std::string const & username, std::string const & hostname,
                  std::string const & servername, std::string realname) {

    if (realname[0] != ':')
        clerr(ERR_NEEDMOREPARAMS);
    realname.erase(0, 1);
    if (realname.size() > 50)
        clerr(ERR_NEEDMOREPARAMS);
    else if (_isRegistered)
        clerr(ERR_ALREADYREGISTRED);
    else if (_nickName.empty())
        clerr(ERR_NONICKNAMEGIVEN);
    
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
    return _serverName == "" ? "*" : _serverName;
}

std::string Client::getRealName() const {
    return _realName;
}


bool Client::isNicked() const {
	return _nickName != "";
}

bool Client::isServerNamed() const {
    return _serverName != "";
}


void Client::sendMsgToClient(Client const & client, std::string const & msg) const {
    std::string data = ":" + getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " " + msg + "\r\n";

    std::cout << BLUE << getNickName() << " -> " << client.getNickName() << WHITE ": |" CYAN << data << WHITE "|" << std::endl; 
    Server::sendData(client, data);
}

void Client::sendMsgToClientsChannel(Channel const & channel, std::string const & msg, client_map const & clients, bool toMe) const {
    std::list<Client *> channelClients = Server::filterClientsByFd(clients, channel.getUserList());
    

    for (std::list<Client*>::const_iterator client = channelClients.begin(); client != channelClients.end(); client++)
        {
            if (*client != this || toMe)
                sendMsgToClient(**client, msg);
        }
}

void Client::sendPrivateMsg(Client const & receiver, std::string const & msg) const {
    
    std::string data = "PRIVMSG " + receiver.getNickName() + " :" + msg;
    sendMsgToClient(receiver, msg); 
}

void Client::sendPrivateMsg(Channel const & channel, std::string const & msg, client_map const & clients) const {
        
    std::vector<int> channelFds = channel.getUserList();
    std::vector<int> channelOps = channel.getOperatorList();

    if (find(channelFds.begin(), channelFds.end(), _fd) == channelFds.end())
        clerr(ERR_CANNOTSENDTOCHAN);
    if (channel.isModerated() && find(channelOps.begin(), channelOps.end(), _fd) == channelOps.end())
        clerr(ERR_CANNOTSENDTOCHAN);

    std::string data = "PRIVMSG " + channel.getName() + " :" + msg;
    sendMsgToClientsChannel(channel, data, clients, false);
}