#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# pragma once

#include <vector>
#include <string>
#include "client.hpp"

class Client;

class	Channel {
	public :
		Channel();
		Channel(std::string name, std::string const &topic, Client const &creator); 

		Channel & operator=(Channel const & rhs);

		//getters
		std::string		getName() const;
		std::string		getTopic() const;
		
		//creator needed to set the first operator
	private :
	//variables
		std::string	_name;
		std::string	_topic; //max 200 characters
		std::string	_password; //only used if keyMask is set
		int			_userCount; //number of users in the channel
		int			_userLimit; //only used if userLimitMask is set
		//Masks
		bool		_banMask; //when set, only users outside of the ban list can join
		bool		_inviteMask; //when set, only invited users can join
		bool		_voiceMask; //when set, only voiced users can speak
		bool 		_keyMask; //when set, only users with the correct key can join
		bool		_privateMask; //when set, channel topic won't be displayed in the channel list 
		bool		_secretMask; //when set, channel name won't be displayed in the channel list nor in the WHO list
		bool		_userLimitMask; //when set, only users up to the user limit can join
		//Lists
		std::vector<int>	_userList; //list of users in the channel
		std::vector<int>	_banList; //list of users banned from the channel
		std::vector<int>	_inviteList; //list of users invited to the channel
		std::vector<int>	_operatorList; //list of users with operator status
		std::vector<int>	_voiceList; //list of users with voice status
};

#endif