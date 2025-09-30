/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:44:45 by arthur            #+#    #+#             */
/*   Updated: 2025/09/30 14:47:31 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Exception.hpp"
# include "Client.hpp"
# include "Lang.hpp"
# include "Server.hpp"
# include <iostream>
# include <netinet/in.h>
# include <poll.h>
# include <unistd.h>
# include <sys/socket.h>
# include <vector>
