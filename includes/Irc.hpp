/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emaillet <emaillet@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:44:45 by arthur            #+#    #+#             */
/*   Updated: 2025/10/01 16:58:21 by emaillet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* ************************************************************************** */
/* Include extern header */
/* ************************************************************************** */

# include <iostream>
# include <netinet/in.h>
# include <poll.h>
# include <unistd.h>
# include <sys/socket.h>
# include <vector>
# include <cstring>
# include <cstdlib>
# include <streambuf>
#include <sstream>

/* ************************************************************************** */
/* Includes headers */
/* ************************************************************************** */

# include "Errors/Lang.hpp"
# include "Server/FdOutBuf.hpp"
//# include "Errors/Exception.hpp"
//# include "Server/Server.hpp"
// # include "Clients/Client.hpp"

/* ************************************************************************** */
/* Other functions */
/* ************************************************************************** */

bool	checkPass(int clientSocket, std::string password);

/* ************************************************************************** */
/* End of file */
/* ************************************************************************** */
