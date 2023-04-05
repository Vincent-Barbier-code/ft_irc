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
	_moderatedMask = false;
}

Channel::Channel(std::string name, std::string const &topic, Client const &creator) {
    _name = name;
    _topic = topic;
    _password = "";
    _userCount = 1;
    _userLimit = 0;
    _banMask = false;
    _inviteMask = false;
    _voiceMask = false;
    _keyMask = false;
    _privateMask = false;
    _secretMask = false;
    _userLimitMask = false;
	_moderatedMask = false;
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

Channel::~Channel() {
}

bool Channel::operator==(Channel const & rhs) const {
    return _name == rhs._name;
}

bool Channel::operator==(std::string const & rhs) const {
    return _name == rhs;
}

bool	Channel::isClientInList(std::vector<int> list, int fdClient) const {
	std::vector<int>::iterator it = list.begin();
	std::vector<int>::iterator ite = list.end();

	while (it != ite) {
		if (*it == fdClient)
			return (true);
		it++;
	}
	return (false);
}

bool	Channel::addClientToList(std::vector<int> list, int fdClient) {
	if (isClientInList(list, fdClient))
		return (false);
	list.push_back(fdClient);
	return (true);
}

bool	Channel::rmClientFromList(std::vector<int> list, int fdClient) {
	std::vector<int>::iterator it = list.begin();
	std::vector<int>::iterator ite = list.end();

	while (it != ite) {
		if (*it == fdClient) {
			list.erase(it);
			return (true);
		}
		it++;
	}
	return (false);
}

//GETTERS
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

int Channel::getbanMask() const {
    return _banMask;
}

std::vector<int> Channel::getOperatorList() const {
    return _operatorList;
}

std::vector<int> Channel::getUserList() const {
    return _userList;
}

std::vector<int>	Channel::getBanList() const {
	return _banList;
}

std::vector<int>	Channel::getOpList() const {
	return _operatorList;
}

void Channel::addUser(int fd) {
    _userList.push_back(fd);
}

void Channel::addOperator(int fd) {
    _operatorList.push_back(fd);
}

void Channel::addBan(int fd) {
    _banList.push_back(fd);
}

void Channel::addInvite(int fd) {
    _inviteList.push_back(fd);
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

int Channel::isInUserList(int fd) const {
    for (std::vector<int>::const_iterator it = _userList.begin(); it != _userList.end(); it++) {
        if (*it == fd)
            return 1;
    }
    return 0;
}

int Channel::isInOperatorList(int fd) const {
    for (std::vector<int>::const_iterator it = _operatorList.begin(); it != _operatorList.end(); it++) {
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

bool Channel::isModerated() const {
    return false;
}



//SETTERS

void	Channel::setBanMask(bool mode) {
	_banMask = mode;
}

void	Channel::setModeratedMask(bool mode) {
	_banMask = mode;
}

void	Channel::setInviteMask(bool mode) {
	_inviteMask = mode;
}

void	Channel::setVoiceMask(bool mode) {
	_voiceMask = mode;
}

void	Channel::setKeyMask(bool mode) {
	_keyMask = mode;
}

void	Channel::setPrivateMask(bool mode) {
	_privateMask = mode;
}

void	Channel::setSecretMask(bool mode) {
	_secretMask = mode;
}

void	Channel::setUserLimitMask(bool mode) {
	_userLimitMask = mode;
}

void	Channel::setKey(std::string key) {
	_password = key;
}

void	Channel::setTopic(std::string topic) {
    _topic = topic;
}

bool	Channel::setUserLimit(std::string limit) {
	unsigned long int tmp_nb;
	int	i = 0;
	
	while(limit[i]) {
		if (!isdigit(limit[i]))
			return (false);
		i++;
	}
	if (i > 10)
		return (false);
	tmp_nb = atoi(limit.c_str());
	if (tmp_nb > 2147483647)
		return (false);
	_userLimit = tmp_nb;
	return (true);
}
