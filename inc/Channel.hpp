#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# pragma once

#include "macro.hpp"
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
		std::string			getName() const;
		std::string			getTopic() const;
		std::vector<int>	getUserList() const;
		std::vector<int>	getBanList() const;
		std::vector<int>	getOpList() const;
		bool				isClientInList(std::vector<int> list, int fdClient) const;
		
		//setters
		void				setBanMask(bool mode);
		void				setInviteMask(bool mode);
		void				setVoiceMask(bool mode);
		void				setKeyMask(bool mode);
		void				setPrivateMask(bool mode);
		void				setSecretMask(bool mode);
		void				setUserLimitMask(bool mode);
		void				setModeratedMask(bool mode);
		void				setKey(std::string key);
		bool				setUserLimit(std::string limit);

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
		bool		_voiceMask; //when set, only voiced users (operators) can speak
		bool 		_keyMask; //when set, only users with the correct key can join
		bool		_privateMask; //when set, channel topic won't be displayed in the channel list 
		bool		_secretMask; //when set, channel name won't be displayed in the channel list nor in the WHO list
		bool		_userLimitMask; //when set, only users up to the user limit can join
		bool		_moderatedMask;
		//Lists
		std::vector<int>	_userList; //list of users in the channel
		std::vector<int>	_banList; //list of users banned from the channel
		std::vector<int>	_inviteList; //list of users invited to the channel
		std::vector<int>	_operatorList; //list of users with operator status
		std::vector<int>	_voiceList; //list of users with voice status
};

#endif