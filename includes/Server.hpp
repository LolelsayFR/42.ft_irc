/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:52:45 by emaillet          #+#    #+#             */
/*   Updated: 2025/09/30 10:53:15 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <netinet/in.h>

class Server {
private:
	Server();
	const int			_port;
	const std::string	_password;
    //sockaddr_in			_serverAddress;
	//std::vector<Client&>
	//std::vector<Channel&>
public:
	Server(int port, std::string password);
	~Server(void);
	void ServetStart(void);
};

#endif // SERVER_HPP