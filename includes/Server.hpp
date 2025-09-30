/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:52:45 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 11:50:21 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <netinet/in.h>

class Server {
private:
	const int			_port;
	const std::string	_password;
	Server(void);
	Server(const Server& other);
	Server& operator=(const Server& other);
    //sockaddr_in			_serverAddress;
	//std::vector<Client&>
	//std::vector<Channel&>
public:
	//All constructor and destructor
	Server(int port, std::string password);
	~Server(void);
	//All other member functions
	void start(void);
};

#endif // SERVER_HPP