/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:43:51 by arthur            #+#    #+#             */
/*   Updated: 2025/10/01 10:27:50 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Irc.hpp"

class Client {
private :
	const int			_uid;
	std::string			_username;
	std::string			_nickname;
	//std::ostream		_clientStream;
	//std::vector<Channel&> <--- All channel joined;
public :
	Client(int fd);
	~Client(void);
	// const int getUid(void)const;
	std::string getUsername(void) const;
	std::string getNickname(void) const;
	void setNickname(std::string nickname);
	void setUsername(std::string username);
	void extractNickname(const std::string& buffer);
	void extractUsername(const std::string& buffer);

	// std::string getNickname(void)const;
	// void setNickname(std::string nickname);
	//Client Command
	//void join(std::string channel);
};
