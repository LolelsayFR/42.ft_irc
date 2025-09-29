/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_class.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:44:33 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/29 18:35:53 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netinet/in.h>

class Server {
private:
	Server();
	const int			_port;
	const std::string	_password;
    sockaddr_in			_serverAddress;
	//std::vector<Client&>
	//std::vector<Channel&>
public:
	Server(int port, std::string password);
	~Server(void);
	void ServetStart(void);
};

class Channel {
private:
	const int					_cid;
	const std::string	_name;
	std::string			_topic;
	std::string			_password; //IF EMPTY == NONE
	bool				_needInvite;
	//std::map[class user] <--- All user joined;
public:
};

class Client {
private :
	const int			_uid;
	const std::string	_username;
	std::string	_nickname;
	//std::vector<Channel&> <--- All channel joined;
public :
	Client(void);
	~Client(void);
	const int getUid(void)const;
	const std::string getUsername(void)const;
	std::string getNickname(void)const;
	void setNickname(std::string nickname);
	//Client Command
	void join(std::string channel);
};