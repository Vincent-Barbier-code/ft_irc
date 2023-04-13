#include "Client.hpp"

//--------------Setters-----------------

std::string Client::setNickName(std::string const &nickName)
{
	this->_nickName = nickName;
	return (this->_nickName);
}

void Client::clearBuf() {
    _buf.clear();
}

void Client::setBuf(std::string const & buf) {
    _buf = buf;
}

void Client::appendBuf(char const * buf, size_t len) {
    _buf.append(buf, len);
}

//---------------Getters-----------------

int Client::getFd() const
{
	return (this->_fd);
}

sockaddr_in Client::getAddr()
{
	return (this->_addr);
}

std::string Client::getUserName() const
{
	return (this->_userName);
}

std::string Client::getNickName() const
{
	return (this->_nickName == "" ? "*" : this->_nickName);
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

std::string Client::getBuf() const {
    return _buf;
}

bool Client::isNicked() const {
	return _nickName != "";
}

bool Client::isServerNamed() const {
    return _serverName != "";
}

bool Client::isConnected() const {
	int error = 0;
	socklen_t len = sizeof(error);
	if (getsockopt(_fd, SOL_SOCKET, SO_ERROR, &error, &len) != 0) {
		return false;
	}
	if (error != 0)
		return false;
    return true;
}

bool Client::isRegistered() const {
	return (_isRegistered);
}

bool Client::isAuth() const {
	return (_isAuth);
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