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


void Client::sendMsgToCLient(Client const & client, std::string const & msg) const {
    std::string data = ":" + getNickName() + " " + msg + "\r\n";

    std::cout << BLUE << getNickName() << " -> " << client.getNickName() << WHITE ": |" CYAN << data << WHITE "|" << std::endl; 
    Server::sendData(client, data);
}

void Client::sendMsgToClientsChannel(Channel const & channel, std::string const & msg, client_map const & clients, bool toMe) const {
    std::list<Client *> channelClients = Server::filterClientsByFd(clients, channel.getUserList());
    

    for (std::list<Client*>::const_iterator client = channelClients.begin(); client != channelClients.end(); client++)
        {
            if (*client != this || toMe)
                sendMsgToCLient(**client, msg);
        }
}

void Client::sendPrivateMsg(Client const & receiver, std::string const & msg) const {
    
    std::string data = "PRIVMSG " + receiver.getNickName() + " :" + msg;
    sendMsgToCLient(receiver, msg); 
}

void Client::sendPrivateMsg(Channel const & channel, std::string const & msg, client_map const & clients) const {
        
    std::vector<int> channelFds = channel.getUserList();
    std::vector<int> channelOps = channel.getOperatorList();

    if (find(channelFds.begin(), channelFds.end(), _fd) == channelFds.end())
        clerr(ERR_CANNOTSENDTOCHAN);
    if (channel.getVoiceMask() && find(channelOps.begin(), channelOps.end(), _fd) == channelOps.end())
        clerr(ERR_CANNOTSENDTOCHAN);

    std::string data = "PRIVMSG " + channel.getName() + " :" + msg;
    sendMsgToClientsChannel(channel, data, clients, false);
}

void Client::sendMsgToStalkers(std::string const & msg, channel_map const & channels, client_map const & clients) const {
    std::set<Client const *> stalkers = getStalkers(channels, clients);
    for (std::set<Client const *>::const_iterator stalker = stalkers.begin(); stalker != stalkers.end(); stalker++)
        sendMsgToCLient(**stalker, msg); 
}

std::set<Client const *> Client::getStalkers(channel_map const & channels, client_map const & clients) const {
    std::set <Client const *> stalkers;
    std::list<Channel const *> joinedChannels = getJoinedChannels(channels);

    for (std::list<Channel const *>::const_iterator channel = joinedChannels.begin(); channel != joinedChannels.end(); channel++) {
        std::vector<int> const & users = (*channel)->getUserList();
        for (std::vector<int>::const_iterator user = users.begin(); user != users.end(); user++)
            if (*user != _fd)
                stalkers.insert(clients.at(*user));
    }
    return stalkers;
}

std::list<Channel const *> Client::getJoinedChannels(channel_map const & channels) const {
    std::list<Channel const *> joinedChannels;
    for (channel_map::const_iterator channel = channels.begin(); channel != channels.end(); channel++) {
        
        std::vector<int> const & users = channel->second.getUserList();
        if (find(users.begin(), users.end(), _fd) != users.end()) {
            joinedChannels.push_back(&(channel->second));
        }
    }

    return joinedChannels;
}