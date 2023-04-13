#include "Client.hpp"
#include "Channel.hpp"

//---------Constructors - Destructors-----------
Client::Client(): _fd(0), _addr(), _nickName(), _userName(), _isRegistered(false), _isAuth(false), _isInvisible(false)
{
}

Client::Client(int fd, sockaddr_in addr) : _fd(fd), _addr(addr), _nickName(), _userName(), _isRegistered(false), _isAuth(false), _isInvisible(false)
{

}

Client::Client(int fd, sockaddr_in addr, std::string nickName) : _fd(fd), _addr(addr), _nickName(nickName), _userName(), _isRegistered(false), _isAuth(false), _isInvisible(false)
{

}

Client::~Client()
{
}

bool Client::operator==(Client const & rhs) const
{
	return (this->_nickName == rhs._nickName);
}


//---------------Messages-----------------------
void Client::sendMsgToClient(Client const & client, std::string const & msg) const {
    std::string data = ":" + getNickName() + "!" + getUserName() + "@" + getHostName() + " " + msg + "\r\n";

    std::cout << BLUE << getNickName() << " -> " << client.getNickName() << WHITE ": |" CYAN << data << WHITE "|" << std::endl; 
    Server::sendData(client, data);
}

void Client::botSendMsgToClient(Client const & client, std::string const & msg) const {
    std::string data = ":bipboup " + msg + "\r\n";

    Server::sendData(client, data);
}

void Client::sendMsgToClientsChannel(Channel const & channel, std::string const & msg, client_map const & clients, bool toMe) const {
    std::list<Client *> channelClients = Server::filterClientsByFd(clients, channel.getUserList());

    for (std::list<Client*>::const_iterator client = channelClients.begin(); client != channelClients.end(); client++) {
        if (*client != this || toMe)
            sendMsgToClient(**client, msg);
    }
}

void Client::sendMsgToStalkers(std::string const & msg, channel_map const & channels, client_map const & clients) const {
    std::set<Client const *> stalkers = getStalkers(channels, clients);
    for (std::set<Client const *>::const_iterator stalker = stalkers.begin(); stalker != stalkers.end(); stalker++)
        sendMsgToClient(**stalker, msg); 
}
