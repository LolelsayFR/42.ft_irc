/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:43:51 by arthur            #+#    #+#             */
/*   Updated: 2025/09/30 16:08:28 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Irc.hpp"

class Client {
private :
	const int			_uid;
	std::string	_username;
	std::string			_nickname;
	//std::ostream		_clientStream;
	//std::vector<Channel&> <--- All channel joined;
public :
	Client(int fd);
	~Client(void);
	// const int getUid(void)const;
	std::string getUsername(void)const;
	void setUsername(std::string username);

	// std::string getNickname(void)const;
	// void setNickname(std::string nickname);
	//Client Command
	//void join(std::string channel);
};
