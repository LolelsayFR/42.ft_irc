/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FdOutBuf.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:04:31 by artgirar          #+#    #+#             */
/*   Updated: 2025/10/01 10:14:19 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/FdOutBuf.hpp"

FdOutBuf::FdOutBuf(void) : _fd(-1) {
}

FdOutBuf::FdOutBuf(int fd) : _fd(fd) {
}

FdOutBuf	&FdOutBuf::operator=(const FdOutBuf & other) {
	if (this != &other)
	{
		this->_fd = other._fd;
	}
	return (*this);
}

FdOutBuf::~FdOutBuf(void)
{
}

FdOutBuf::int_type FdOutBuf::overflow(int_type c) {
        if (c != -1) {
            char z = c;
            if (write(_fd, &z, 1) != 1) {
                return -1;
            }
        }
        return c;
}

std::streamsize FdOutBuf::xsputn(const char* s, std::streamsize num) {
        return (write(_fd, s, num));
}
