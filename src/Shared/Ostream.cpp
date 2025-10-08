/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ostream.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 12:12:51 by emaillet          #+#    #+#             */
/*   Updated: 2025/10/08 12:46:52 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Irc.hpp"
#include "Client/Client.hpp"
#include "Server/Server.hpp"
#include "Channel/Channel.hpp"

/* ************************************************************************** */
/* All ostream overload */
/* ************************************************************************** */


//Server Overload
std::ostream& operator<<(std::ostream& o, Server& s) {
	Client* ptr;
	o << std::endl << WHI << "/* Server View * */" << RES << std::endl;
	o 	<< " /\tPort : " << s.getPort()
		<< "\n /\tPassword : " << s.getPassword()
		<< "\n /\tHost : " << s.getHost() << std::endl
		<< WHI << "/* ********* */" << RES << std::endl;
	{
		std::vector<Client*> list = s.getClientList();
		std::vector<Client*>::iterator	it = list.begin();
		std::vector<Client*>::iterator	end = list.end();
		o << std::endl << WHI <<  "/* Client connected * */" << RES << std::endl;
		if (list.empty())
			o << " /\tEmpty.." << std::endl;
		else
			for (int i = 0; it != end; i++, it++) {
				ptr = *it;
				{
				o 	<< "| n°" << std::setw(3) << i
					<< " | U = " <<  std::setw(10) << ptr->getUsername()
					<< " | N = " <<  std::setw(10) << ptr->getNickname()
					<< " | R = " <<  std::setw(20) << ptr->getRealname()
					<< " | H = " <<  std::setw(10) << ptr->getHostname()
					<< " |"  << std::endl;
				}
			}
		o << WHI << "/* ********* */" << RES << std::endl;
	}
	{
		std::vector<Client*> list = s.getSetupList();
		std::vector<Client*>::iterator	it = list.begin();
		std::vector<Client*>::iterator	end = list.end();
		o << std::endl <<  WHI << "/* Client in setup * */" << RES << std::endl;
		if (list.empty())
			o << " /\tEmpty.." << std::endl;
		else
			for (int i = 0; it != end; i++, it++) {
				ptr = *it;
				{
				o 	<< "| n°" << std::setw(3) << i
					<< " | ID = " <<  std::setw(5) << ptr->getUid()
					<< " |"  << std::endl;
				}
			}
		o << WHI << "/* ********* */" << RES << std::endl;
	}
	{
		std::vector<Channel*> list = s.getChannelList();
		std::vector<Channel*>::iterator	it = list.begin();
		std::vector<Channel*>::iterator	end = list.end();
		for (int i = 0; it != end; i++, it++)
			o << *(static_cast<Channel*>(*it));
	}
	return (o);
}

//Channel overload
std::ostream& operator<<(std::ostream& o, Channel& c) {
	Client* ptr;
	o << std::endl << WHI << "/* Channel View */" << RES << std::endl;
	{
		std::vector<Client*> list = c.getJoinList();
		std::vector<Client*>::iterator	it = list.begin();
		std::vector<Client*>::iterator	end = list.end();
		o << " /\tChannel name : " << c.getName() << std::endl;
		if (c.getNeedPassword())
			o << " /\tNeed password : true, the pass is '" << c.getPassword() << "'" << std::endl;
		else
			o << " /\tNeed password : false" << std::endl;
		o << " /\tChannel topic : " << c.getTopic() << std::endl;
		o << WHI << "/* Client in channel */" << RES << std::endl;
		for (int i = 0; it != end; i++, it++) {
			ptr = *it;
			{
			o 	<< " | n°" << std::setw(5) << i
				<< " | Username = " <<  std::setw(10) << ptr->getUsername()
				<< " | Nickname = " <<  std::setw(10) << ptr->getNickname()
				<< " | Is op = " << ((c.findClientOp(*ptr) == -1) ? ("false") : ("true "))
				<< " |"  << std::endl;
			}
		}
		if (list.empty())
			o << "/\tEmpty.." << std::endl;
		o << WHI << "/* ********** */" << RES << std::endl;
	}
	if (!c.getNeedInvite())
		return (o);
	{
		std::vector<Client*> list = c.getInviteList();
		std::vector<Client*>::iterator	it = list.begin();
		std::vector<Client*>::iterator	end = list.end();
		o << WHI << "/* Client invitation list from channel */" << RES << std::endl;
		if (list.empty())
			o << "\tEmpty.." << std::endl;
		else
			for (int i = 0; it != end; i++, it++) {
				ptr = *it;
				o 	<< "| n°" << std::setw(5) << i
					<< " | Username = " <<  std::setw(10) << ptr->getUsername()
					<< " | Nickname = " <<  std::setw(10) << ptr->getNickname()
					<< " |"  << std::endl;
			}
		o << WHI << "/* ********* */" << RES << std::endl;
	}
	return (o);
}

/* ************************************************************************** */
/* End of file */
/* ************************************************************************** */