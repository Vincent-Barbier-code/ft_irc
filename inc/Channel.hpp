#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# pragma once

#include "macro.hpp"
#include <vector>
#include <string>
#include "client.hpp"
#include "utils.hpp"
#include "server.hpp"

class Client;

class	Channel {
	public :

		typedef std::map<std::string, Channel> channel_map;

		Channel();
		Channel(std::string name, std::string const &topic, Client const &creator); 

		Channel & operator=(Channel const & rhs);
        ~Channel();
		bool operator==(Channel const & rhs) const;
		bool operator==(std::string const & rhs) const;
		

		//getters
		std::string			getName() const;
		std::string			getTopic() const;
		std::string			getPassword() const;
		int					getkeyMask() const;
		int 				getinviteMask() const;
		bool 				getVoiceMask() const;
		std::vector<int>	getUserList() const;
		std::vector<int>    getOperatorList() const;
		std::vector<int>	getBanList() const;
		std::vector<int>	getOpList() const;
		unsigned long int	getUserLimit() const;
		bool				getSecretMask() const;
		bool				getPrivateMask() const;

		int 		isInUserList(int fd) const;
		int			isInInviteList(int fd) const;
		int 		isInBanList(int fd) const;
		int			isInOperatorList(int fd) const;
		int			isInVoiceList(int fd) const;
		void		addUser(int fd);
		bool		addInvite(int fd);
		bool		addBan(int fd);
		bool		addOperator(int fd);
		bool		addVoice(int fd);
		bool		removeOperator(int fd);
		bool		removeBan(int fd);
		void		removeUser(int fd);
		void		removeInvite(int fd);
		void		removeVoice(int fd);

		bool                isModerated() const;
		
		//creator needed to set the first operator

		bool				isClientInList(std::vector<int> list, int fdClient) const;
		void				setInviteMask(bool mode);
		void				setVoiceMask(bool mode);
		void				setPrivateMask(bool mode);
		void				setKeyMask(bool mode);
		void				setSecretMask(bool mode);
		void				setUserLimitMask(bool mode);
		void				setModeratedMask(bool mode) ;
		void				setKey(std::string key);
		bool				setUserLimit(std::string limit);
		void				setTopic(std::string topic);
	
	private :
	//variables
		std::string			_name;
		std::string			_topic; //max 200 characters
		std::string			_password; //only used if keyMask is set
		unsigned long int	_userLimit; //only used if userLimitMask is set
		//Masks
		bool		_inviteMask; //when set, only invited users can join
		bool		_voiceMask; //when set, only voiced users (operators) can speak
		bool 		_keyMask; //when set, only users with the correct key can join
		bool		_privateMask; //when set, channel topic won't be displayed in the channel list 
		bool		_secretMask; //when set, channel name won't be displayed in the channel list nor in the WHO list
		bool		_userLimitMask; //when set, only users up to the user limit can join
		bool		_moderatedMask; //when set, only operators can speak
		//Lists
		std::vector<int>	_userList; //list of users in the channel
		std::vector<int>	_banList; //list of users banned from the channel
		std::vector<int>	_inviteList; //list of users invited to the channel
		std::vector<int>	_operatorList; //list of users with operator status
		std::vector<int>	_voiceList; //list of users with voice status
};

#endif