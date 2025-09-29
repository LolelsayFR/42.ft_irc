# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ariou <ariou@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/04 17:23:35 by arthur            #+#    #+#              #
#    Updated: 2025/09/29 17:54:32 by ariou            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables

NAME = server

LIBFT = libs/libft

CC = c++

CFLAGS = -Wall -Wextra -Werror -g -std=c++98

GREEN = \033[32m
RED = \033[31m
DEF_COLOR = \033[0m

# Sources

include Sources.mk

SRC_DIR = ./src

SRC = $(SRC_FILES)

OBJS = $(SRC:.cpp=.o)

# Fonctions

all: $(NAME)
	@echo "$(GREEN)Compilation successful !$(DEF_COLOR)"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@ || (echo "$(RED)Error compiling $<!$(DEF_COLOR)" && exit 1)

clean:
	@rm -f $(OBJS)
	@echo "$(GREEN)Clean successful !$(DEF_COLOR)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)Full clean successful !$(DEF_COLOR)"

re: fclean all

.PHONY: all clean fclean re
