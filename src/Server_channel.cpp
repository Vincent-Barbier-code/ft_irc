#include "Server.hpp"

void    Server::addChannel(Channel const & channel) {
    //exemple -> addChannel(Channel("#general", "Welcome to the general channel", client));
    std::cerr << "adding channel: " << channel.getName() << std::endl;
    if (_channels.find(channel.getName()) != _channels.end()) {
        std::cerr << "Channel already exists" << std::endl;
        throw std::runtime_error("addChannel() -> Channel already exists");
    }
    _channels[channel.getName()] = channel;

    std::cerr << "Topic of new channel: " << _channels[channel.getName()].getTopic() << std::endl;
}