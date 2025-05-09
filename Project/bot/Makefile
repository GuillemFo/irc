# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/08 20:21:18 by rzhdanov          #+#    #+#              #
#    Updated: 2025/05/09 23:11:34 by rzhdanov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GCC = g++
FLAGS = -Wall -Werror -Wextra -std=c++98

SRC_DIR = src
CMD_DIR = commands
INC_DIR = include
TEST_DIR = tests

NAME = bot

MAIN = main.cpp

TEST_NAME = bot_test

TEST_MAIN = $(TEST_DIR)/main_test.cpp

SRC = \
	$(SRC_DIR)/Bot.cpp \
	$(CMD_DIR)/BotPingCommand.cpp \

INCLUDES = -I$(INC_DIR) -I$(CMD_DIR)

all: $(NAME)

$(NAME): $(SRC) $(MAIN)
	$(GCC) $(FLAGS) $(SRC) $(MAIN) -o $(NAME) $(INCLUDES)

$(TEST_NAME): $(SRC) $(TEST_MAIN)
	$(GCC) $(FLAGS) $(SRC) $(TEST_MAIN) -o $(TEST_NAME) $(INCLUDES)

clean:
	rm -f $(NAME) $(TEST_NAME)

fclean: clean

re: fclean all

.PHONY:l all clean fclean re