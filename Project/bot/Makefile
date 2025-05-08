# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/08 20:21:18 by rzhdanov          #+#    #+#              #
#    Updated: 2025/05/08 22:02:00 by rzhdanov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GCC = g++
FLAGS = -Wall -Werror -Wextra -std=c++98

SRC_DIR = src
CMD_DIR = commands
INC_DIR = include

NAME = bot

SRC = \
	$(SRC_DIR)/bot.cpp \
	$(CMD_DIR)/PingCommand.cpp \
	main.cpp

INCLUDES = -I$(INC_DIR) -I$(CMD_DIR)

all: $(NAME)

$(NAME): $(SRC)
	$(GCC) $(FLAGS) $(SRC) -o $(NAME) $(INCLUDES)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all
