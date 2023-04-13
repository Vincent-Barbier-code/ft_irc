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

//----------------Commands-----------------------
//Commande : PASS
//Paramètres : <password>
void	Client::pass(std::string const &clientPass, std::string const &serverPass) {
	if (_isAuth)
		clerr(ERR_ALREADYREGISTRED);
	else if (clientPass == serverPass) {
		_isAuth = true;
		std::cout << "Client " << inet_ntoa(_addr.sin_addr) << " is now authentified" << std::endl;
	}
	else {
		std::cout << RED "Password mismatch" WHITE << std::endl;
		clerr(ERR_PASSWDMISMATCH);
	}
}

//Commande : MODE
// Parametres 1 : <canal> {[+|-]|o|p|s|i|t|n|b|v} [<limite>] [<utilisateur>] [<masque de bannissement >]
// Parametres 2 : <pseudonyme> {[+|-]i}
void	Client::modeUser(std::string const name, std::string const mode, Client & client) {
	if (name != _nickName)
		clerr(ERR_USERSDONTMATCH);
	if (mode[1] == 'i') {
		if (mode[0] == '+')
			_isInvisible = true;
		else
			_isInvisible = false;
		sendMsgToClient(client, "MODE " + _nickName + " :" + mode);
	}
	else
		clerr(ERR_UMODEUNKNOWNFLAG);
}

void Client::user(std::string const & username, std::string const & hostname,
                  std::string const & servername, std::string realname) {

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
    

    for (std::list<Client*>::const_iterator client = channelClients.begin(); client != channelClients.end(); client++)
        {
            if (*client != this || toMe)
                sendMsgToClient(**client, msg);
        }
}

void Client::sendPrivateMsg(Client const & receiver, std::string const & msg, bool isNotice/* = false*/) const {
    
    std::string cmd = isNotice ? "NOTICE" : "PRIVMSG";
    std::string data = cmd + " " + receiver.getNickName() + " :" + msg;
    sendMsgToClient(receiver, data); 
}

void Client::sendPrivateMsg(Channel const & channel, std::string const & msg, client_map const & clients) const {
    std::string	morseMsg = "";    
    std::vector<int> channelFds = channel.getUserList();

    if (find(channelFds.begin(), channelFds.end(), _fd) == channelFds.end())
        clerr(ERR_CANNOTSENDTOCHAN);
    if (channel.getModeratedMask() && channel.isInVoiceList(_fd) == false) {
        sendMsgToClient(*this, itostr(ERR_CANNOTSENDTOCHAN) + " " + _nickName + " " + channel.getName() + " :Cannot talk in channel (+m)");
        return;
    }
    std::string data = "PRIVMSG " + channel.getName() + " :" + msg;
    morseMsg = _lookForMorse(msg);
	if (morseMsg.size())
		_morse(channel, morseMsg);
	sendMsgToClientsChannel(channel, data, clients, false);
}

void Client::sendMsgToStalkers(std::string const & msg, channel_map const & channels, client_map const & clients) const {
    std::set<Client const *> stalkers = getStalkers(channels, clients);
    for (std::set<Client const *>::const_iterator stalker = stalkers.begin(); stalker != stalkers.end(); stalker++)
        sendMsgToClient(**stalker, msg); 
}
