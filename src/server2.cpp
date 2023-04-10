#include "server.hpp"

void    Server::sendData(Client const & client, std::string const & data) {

    //std::cout << "send data to " << client_fd << std::endl;
    //std::cout << "data: " << data << std::endl;
    if (send(client.getFd(), data.c_str(), data.size(), 0) == -1) {
        perror("send error");
        throw std::runtime_error("send error");
    }
}

void Server::_sendMsgToClient(Client const & client, std::string const & msg) const {
    std::string data = ":" + client.getServerName() + " " + msg + "\r\n";

    std::cout << "To:" <<  BLUE << client.getNickName() << WHITE " |" CYAN << data << WHITE "|" << std::endl; 
    sendData(client, data);
}

void	Server::_sendNumericReply(int code, Client const & client){
	std::string message = _getNumericReplyMessage(code, client);
	//std::cout << "Sending message to client " << client.getNickName() << ": " << message << std::endl;

	_sendMsgToClient(client, message);
}


void 		Server::_sendWelcomeMsg(Client const & client) {
	std::cout << "Welcoming: "; 
    _sendNumericReply(RPL_WELCOME, client);

}

void    Server::addChannel(Channel const & channel) {
    //exemple -> addChannel(Channel("#general", "Welcome to the general channel", client));
    std::cout << "adding channel: " << channel.getName() << std::endl;
    if (_channels.find(channel.getName()) != _channels.end()) {
        std::cout << "Channel already exists" << std::endl;
        throw std::runtime_error("addChannel() -> Channel already exists");
    }
    _channels[channel.getName()] = channel;

    std::cout << "Topic of new channel: " << _channels[channel.getName()].getTopic() << std::endl;
}

void  Server::_list(Client const & client) {
    std::cout << "Listing channels" << std::endl;
    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        std::cout << it->first << " => " << it->second.getName() << std::endl;
    }
    _sendListStart(client);
    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->second.getSecretMask()) { //if secret channel
            if (it->second.isClientInList(it->second.getUserList(), client.getFd())) //check if user is in channel
                _sendList(client, it->second);
        }
        else if (it->second.getPrivateMask()) { //if private channel
            if (it->second.isClientInList(it->second.getUserList(), client.getFd())) //check if user is in channel
                _sendList(client, it->second);
            else
                _sendListPrivate(client, it->second);
        }
        else //if public channel
            _sendList(client, it->second);
    }
    _sendListEnd(client);
}

void Server::_sendListStart(Client const & client) {
    std::string msg = itostr(RPL_LISTSTART) + " " + client.getNickName() + " Channel :Users Name";
    _sendMsgToClient(client, msg);
}

void Server::_sendList(Client const & client, Channel const & channel) {
    std::string msg = itostr(RPL_LIST) + " " + client.getNickName() + " " + channel.getName() + " " + itostr(channel.getUserList().size()) + " :" + channel.getTopic();
    _sendMsgToClient(client, msg);
}

void Server::_sendListPrivate(Client const & client, Channel const & channel) {
    std::string msg = itostr(RPL_LIST) + " " + client.getNickName() + " " + channel.getName() + " " + itostr(channel.getUserList().size()) + " :" + "Private";
    _sendMsgToClient(client, msg);
}

void Server::_sendListEnd(Client const & client) {
    std::string msg = itostr(RPL_LISTEND) + " " + client.getNickName() + " End of /LIST";
    _sendMsgToClient(client, msg);
}

void Server::_sendPrivateMsg(Client const & sender, std::string const & dests, std::string const & msg) const {
    std::cout << "sendMsg: " << msg << std::endl;

    std::vector<std::string> destsList = ke_split(dests, std::string(","));
    if (destsList.size() > 30)
        clerr(ERR_TOOMANYTARGETS);
    for (std::vector<std::string>::const_iterator dst = destsList.begin(); dst != destsList.end(); ++dst) {
        if (dst->at(0) == '#' || dst->at(0) == '&') {
            if (_channels.find(*dst) == _channels.end())
                clerr(ERR_NOSUCHNICK);
            sender.sendPrivateMsg(_channels.at(*dst), msg, _clients);
        } else {
            Client * client = _findClientByNickName(*dst);
            if (client == NULL)
                clerr(ERR_NOSUCHNICK);
            sender.sendPrivateMsg(*client, msg);
        }
    }
}