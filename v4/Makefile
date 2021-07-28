# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ajeanett <ajeanett@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/09 15:15:25 by ajeanett          #+#    #+#              #
#    Updated: 2021/07/27 16:35:46 by ajeanett         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
SRCS =	main.cpp \
		server_engine/ServerEngine.cpp \
		parse_config/LocationData.cpp \
		parse_config/ServerData.cpp \
		parse_config/ParserConfig.cpp \
		server_engine/Date.cpp \
		HTTP/Request.cpp \
		HTTP/Response.cpp \
		HTTP/Responders/GetResponder.cpp

OBJECTS = $(SRCS:.cpp=.o)
#FLAGS = clang++ -Wall -Wextra -Werror
FLAGS = clang++ -Wall

all: $(NAME)

$(NAME): $(OBJECTS)
	clang++ $(OBJECTS) -o $(NAME)

%.o: %.cpp %.hpp
	$(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re %.o test
