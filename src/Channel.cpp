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

bool Channel::operator==(Channel const & rhs) const {
    return _name == rhs._name;
}

bool Channel::operator==(std::string const & rhs) const {
    return _name == rhs;
}

std::string Channel::getName() const {
    return _name;
}

std::string Channel::getTopic() const {
    return _topic;
}

std::vector<int> Channel::getUserList() const {
    return _userList;
}

std::vector<int> Channel::getOperatorList() const {
    return _operatorList;
}

bool Channel::isModerated() const {
    return false;
}