/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvue <mvue@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 17:58:56 by vbarbier          #+#    #+#             */
/*   Updated: 2023/04/03 19:58:16 by mvue             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
# pragma once

#include "macro.hpp"
#include "ft_irc.hpp"
#include "utils.hpp"
#include "client.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include <iomanip>
#include "Channel.hpp"
#include <algorithm>
#include <list>

class Client;
class Channel;

class  Server {

  public:
	
	static void	sendData(Client const & client, std::string const & data);

	// template <typename clients_ctn, typename fds_ctn>
	static std::list<Client *> filterClientsByFd(std::map<int, Client *> const &clients, std::vector<int> const &fds);

	Server(int port, std::string password);
	~Server();
	
	int 		getFd() const;
	sockaddr_in getAddr() const;
	std::string	getPass() const;
	void 		start(void);
	void 		stop(void);


	void 		addChannel(Channel const & channel);
	void		mode(std::string const name, std::string const mode, std::string option, Client & client);

  private:
  	int								_server_fd;
	sockaddr_in						_addr;
	int								_epoll_fd;
	std::map<int, Client *>			_clients;
	std::string						_password;
	std::map<std::string, Channel>	_channels;
	
	Server();

	void		_initEpoll(void);
	void		_acceptNewConnection(void);
	void		_treatClientEvent(epoll_event const & client_ev);
	void		_deconnection(int client_fd);
	Client		*_findClientByFd(int fd);
	Client		*_findClientByNickName(std::string const nickName) const;
	int			_clientConnect(int client_fd, sockaddr client_addr, std::string username, std::string nickname);
	void		_execRawMsgs(std::string const & raw_msgs, int client_fd);
	void		_execute(Client & client, Message const & msg);

	std::string _getNumericReplyMessage(int code, Client const &client) const;
	void		_sendNumericReply(int code, Client const & client);
	void 		_sendMsgToCLient(Client const & client, std::string const & msg) const;
	void 		_sendMsgToClientsChannel(Channel const & channel, std::string const & msg) const;
	void        _sendMsgNumericToCLient(Client const & client, int code, std::string const & msg);
	void 		_sendWelcomeMsg(Client const & client);

	//channel
	void    	_createChannel(std::string const & channelName, std::string const & topic, Client const & client);
		
	//CmdServer
	void 		_user(Client & client, std::vector<std::string> const & params);
	void		_parseJoin(int client_fd, std::string const & name);
	void		_nick(int client_fd, std::string const nick);
	void		_join(int client_fd, std::string const & name, std::string const & key);
	void 		_kick(std::string const & channelName, int client_fd, std::string const & comment);
	void		_invite(int client_fd, std::string const & nickName, std::string const & channelName);
	void		_part(int client_fd, std::string const & nameChannel);
	void		_topic(int client_fd, std::string const & nameChannel, std::string const & topic);
	std::string _getUserNameList(Channel channel) const;
	void        _list(Client const & client);
	void		_sendListStart(Client const & client);
	void		_sendList(Client const & client, Channel const & channel);
	void		_sendListEnd(Client const & client);
	void 		_eraseChannel();
	void 		_eraseClient();
	void 		_quit(Client & client, std::string const & comment);
	
	void 		_sendPrivateMsg(Client const & sender, std::string const & dests, std::string const & msg) const;

	void		_modeO(Channel & chan, std::string const mode, std::string option);
	void		_modeB(Channel & chan, std::string const mode, std::string option);
	void		_modeK(Channel & chan, std::string const mode, std::string const option);
	void		_modeChannel(std::string const chanName, std::string const mode, std::string option, Client &client);
	bool		_isClientOp(Channel const & chan, Client &client);
};
#endif

