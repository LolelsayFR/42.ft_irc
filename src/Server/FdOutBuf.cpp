/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FdOutBuf.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artgirar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:04:31 by artgirar          #+#    #+#             */
/*   Updated: 2025/09/30 16:08:02 by artgirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server/FdOutBuf.hpp"

FdOutBuf::FdOutBuf(int fd) : fd_(fd) {
}

FdOutBuf::int_type FdOutBuf::overflow(int_type c) {
        if (c != -1) {
            char z = c;
            if (write(fd_, &z, 1) != 1) {
                return -1;
            }
        }
        return c;
}

std::streamsize FdOutBuf::xsputn(const char* s, std::streamsize num) {
        return write(fd_, s, num);
}
