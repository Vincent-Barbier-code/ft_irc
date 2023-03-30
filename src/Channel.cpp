#include "Channel.hpp"

Channel::Channel() {
    _name = "";
    _topic = "";
    _password = "";
    _userCount = 0;
    _userLimit = 0;
    _banMask = false;
    _inviteMask = false;
    _voiceMask = false;
    _keyMask = false;
    _privateMask = false;
    _secretMask = false;
    _userLimitMask = false;
}

Channel::Channel(std::string name, std::string const &topic, Client const &creator) {
    _name = name;
    _topic = topic;
    _password = "";
    _userCount = 0;
    _userLimit = 0;
    _banMask = false;
    _inviteMask = false;
    _voiceMask = false;
    _keyMask = false;
    _privateMask = false;
    _secretMask = false;
    _userLimitMask = false;
    _userList.push_back(creator.getFd());
    _operatorList.push_back(creator.getFd());
}

Channel & Channel::operator=(Channel const & rhs) {
    _name = rhs._name;
    _topic = rhs._topic;
    _password = rhs._password;
    _userCount = rhs._userCount;
    _userLimit = rhs._userLimit;
    _banMask = rhs._banMask;
    _inviteMask = rhs._inviteMask;
    _voiceMask = rhs._voiceMask;
    _keyMask = rhs._keyMask;
    _privateMask = rhs._privateMask;
    _secretMask = rhs._secretMask;
    _userLimitMask = rhs._userLimitMask;
    _userList = rhs._userList;
    _operatorList = rhs._operatorList;
    return *this;
}

std::string Channel::getName() const {
    return _name;
}

std::string Channel::getTopic() const {
    return _topic;
}

std::string Channel::getPassword() const {
    return _password;
}

int Channel::getkeyMask() const {
    return _keyMask;
}

int Channel::getinviteMask() const{
    return _inviteMask;
}

std::vector<int> Channel::getUserList() const {
    return _userList;
}

void Channel::addUser(int fd) {
    _userList.push_back(fd);
}

void Channel::removeUser(int fd) {
    for (std::vector<int>::iterator it = _userList.begin(); it != _userList.end(); it++) {
        if (*it == fd) {
            _userList.erase(it);
            break;
        }
    }
}

int Channel::isInInviteList(int fd) const {
    for (std::vector<int>::const_iterator it = _inviteList.begin(); it != _inviteList.end(); it++) {
        if (*it == fd)
            return 1;
    }
    return 0;
}

int Channel::isInBanList(int fd) const {
    for (std::vector<int>::const_iterator it = _banList.begin(); it != _banList.end(); it++) {
        if (*it == fd)
            return 1;
    }
    return 0;
}

int Channel::getbanMask() const {
    return _banMask;
}

