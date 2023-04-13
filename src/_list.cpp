#include "Server.hpp"

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