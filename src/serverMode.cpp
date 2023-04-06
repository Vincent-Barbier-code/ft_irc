#include "server.hpp"

void	Server::_modeO(Channel & chan, std::string const mode, std::string const option) {
	std::vector<std::string> options;
	Client	*client_to_op;

	options = ke_split(option, ",");
	if (options.size() > 3)
		clerr(ERR_UNKNOWNMODE);
	std::vector<std::string>::iterator it;
	for (it = options.begin(); it != options.end(); it++) {
		if (!(*it).size())
			clerr(ERR_NEEDMOREPARAMS);
		client_to_op = _findClientByNickName(*it);
		if (!client_to_op)
			clerr(ERR_NOSUCHNICK);
		if (mode[0] == '+') {
			chan.addOperator(client_to_op->getFd());
		}
		else {
			//if last op leaves, oldest user becomes op
			chan.removeOperator(client_to_op->getFd());
			if (chan.getOpList().size() == 0 && chan.getUserList().size() != 0) {
				chan.addOperator(*chan.getUserList().begin());
			}
		}
	}
}

void	Server::_modeB(Channel & chan, std::string const mode, std::string const option) {
	std::vector<std::string> options;
	Client	*client_to_ban;

	options = ke_split(option, ",");
	if (options.size() > 3)
		clerr(ERR_UNKNOWNMODE);
	std::vector<std::string>::iterator it;
	for (it = options.begin(); it != options.end(); it++) {
		if (!(*it).size())
			clerr(ERR_NEEDMOREPARAMS);
		client_to_ban = _findClientByNickName(*it);
		if (!client_to_ban)
			clerr(ERR_NOSUCHNICK);
		if (mode[0] == '+') {
			chan.addBan(client_to_ban->getFd());
			//if client to ban is in channel, kick him
			if (chan.isClientInList(chan.getUserList(), client_to_ban->getFd()))
				_kick(chan.getName(), client_to_ban->getFd(), "You got banned\n");
		}
		else 
			chan.removeBan(client_to_ban->getFd());
	}
}

void	Server::_modeK(Channel & chan, std::string const mode, std::string const option) {
	for (int i = 0; option[i]; i++)
		if (isspace(option[i]))
			clerr(ERR_UNKNOWNMODE);
	if (mode[0] == '+') {
		chan.setKeyMask(true);
		if (option.size())
			chan.setKey(option);
		else 
			clerr(ERR_NEEDMOREPARAMS);
	}
	else
		chan.setKeyMask(false);
}

void	Server::_modeChannel(std::string const chanName, std::string const mode, std::string const option, Client &client) {
	std::map<std::string, Channel>::iterator it;
	Channel &chan = _channels.at(chanName);
	
	if (!mode.size() || (mode[0] != '+' && mode[0] != '-')) {
		clerr(ERR_UNKNOWNMODE);
	}
	if (mode.size() > 2) {
		clerr(ERR_UNKNOWNMODE);
	}
	it = _channels.find(chanName);
	if (it == _channels.end()) {
		clerr(ERR_NOSUCHCHANNEL);
	}
	if (_isClientOp(chan, client)) {
		char c = mode[1];
		switch (c) {
			case 'o':
				_modeO(chan, mode, option);
				client.sendMsgToClientsChannel(chan, "MODE " + chanName + " "  + mode + " " + option, _clients, true);
				break;
			case 'p':
				chan.setPrivateMask(mode[0] == '+');
				client.sendMsgToClientsChannel(chan, "MODE " + chanName + " "  + mode + " ", _clients, true);
				break;
			case 's':
				chan.setSecretMask(mode[0] == '+');
				client.sendMsgToClientsChannel(chan, "MODE " + chanName + " "  + mode + " ", _clients, true);
				break;
			case 'i':
				chan.setInviteMask(mode[0] == '+');
				client.sendMsgToClientsChannel(chan, "MODE " + chanName + " "  + mode + " ", _clients, true);
				break;
			case 'm':
				chan.setModeratedMask(mode[0] == '+');
				client.sendMsgToClientsChannel(chan, "MODE " + chanName + " "  + mode + " ", _clients, true);
				break;
			case 'l':
				if (mode[0] == '+') {
					chan.setUserLimitMask(true);
					if (option.size()) {
						if (!chan.setUserLimit(option))
							clerr(ERR_NEEDMOREPARAMS);
					}
					else
						clerr(ERR_NEEDMOREPARAMS);
				}
				else
					chan.setUserLimitMask(false);
				client.sendMsgToClientsChannel(chan, "MODE " + chanName + " "  + mode + " " + option, _clients, true);
				break;
			case 'b':
				_modeB(chan, mode, option);
				client.sendMsgToClientsChannel(chan, "MODE " + chanName + " "  + mode + " " + option, _clients, true);
				break;
			case 'v':
				chan.setVoiceMask(mode[0] == '+');
				client.sendMsgToClientsChannel(chan, "MODE " + chanName + " "  + mode + " ", _clients, true);
				break;
			case 'k':
				_modeK(chan, mode, option);
				client.sendMsgToClientsChannel(chan, "MODE " + chanName + " "  + mode + " ", _clients, true);
				break;
			default :
				clerr(ERR_UNKNOWNMODE);
		}
	}
}

void Server::mode(std::string const name, std::string const mode, std::string option, Client & client) {
	if (name[0] == '&' || name[0] == '#')
		_modeChannel(name, mode, option, client);
	else
		client.modeUser(name, mode, client);
}