#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# pragma once

class	Channel {
	public :
		Channel();
		Channel(std::string name, std::string const &topic, std::string const &creator);
	private :
		std::string	_name;
		std::string	_topic;
		std::string	_password;
		std::string	_creator;
		int			_userCount;
		int			_userLimit;
		//Masks
		std::string	_banMask; //when set, only users outside of the ban list can join
		std::string	_inviteMask; //when set, only invited users can join
		std::string	_voiceMask; //when set, only voiced users can speak
		std::string _keyMask; //when set, only users with the correct key can join
		std::string	_privateMask; //when set, channel name won't be displayed in the channel list
		std::string	_secretMask; //when set, channel name won't be displayed in the channel list nor in the WHO list
		//Lists
		std::string	_userList; //list of users in the channel
		std::string	_banList; //list of users banned from the channel
		std::string	_inviteList; //list of users invited to the channel
		std::string	_operatorList; //list of users with operator status
		std::string	_voiceList; //list of users with voice status
}

#endif