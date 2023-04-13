#include "Server.hpp"
#include "Client.hpp"

void Server::_sendPrivateMsg(Client const & sender, std::string const & dests, std::string const & msg, bool isNotice /*= false*/) const {

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
            if (client == NULL) {
                if (isNotice)
                    return;
                else
                    clerr(ERR_NOSUCHNICK);
            }
            sender.sendPrivateMsg(*client, msg, isNotice);
        }
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