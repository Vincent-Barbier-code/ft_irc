#include "Server.hpp"

void    Server::sendData(Client const & client, std::string const & data) {

    waitingData[client.getFd()] += data;

    epoll_event client_ev;
    memset(&client_ev, 0, sizeof(client_ev));
    client_ev.data.fd = client.getFd();
    client_ev.events = EPOLLIN | EPOLLRDHUP | EPOLLOUT;
    epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, client.getFd(), &client_ev);
}

void Server::acceptSendData(Client const & client) {
    std::string data = waitingData.at(client.getFd());

    if (send(client.getFd(), data.c_str(), data.size(), 0) == -1) {
        perror("send error");
        throw std::runtime_error("send error");
    }
    waitingData.erase(client.getFd());

    epoll_event client_ev;
    memset(&client_ev, 0, sizeof(client_ev));
    client_ev.data.fd = client.getFd();
    client_ev.events = EPOLLIN | EPOLLRDHUP;
    epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, client.getFd(), &client_ev);
}

void Server::_sendMsgToClient(Client const & client, std::string const & msg) const {
    std::string data = ":" + client.getServerName() + " " + msg + "\r\n";

    std::cerr << "To:" <<  BLUE << client.getNickName() << WHITE " |" CYAN << data << WHITE "|" << std::endl; 
    sendData(client, data);
}

void Server::_sendMsgNumericToCLient(Client const & client, int code, std::string const & msg) {

    std::string data = itostr(code) + " " + client.getNickName() + " " + msg;
    _sendMsgToClient(client, data);
}

void	Server::_sendNumericReply(int code, Client const & client){
	std::string message = _getNumericReplyMessage(code, client);

	_sendMsgToClient(client, message);
}

void Server::_sendMsgToClientsChannel(Channel const & channel, std::string const & msg) const {
    
    std::list<Client *> channelClients = Server::filterClientsByFd(_clients, channel.getUserList());

    for (std::list<Client*>::const_iterator client = channelClients.begin(); client != channelClients.end(); client++)
        _sendMsgToClient(**client, msg);
}