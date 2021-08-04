# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ajeanett <ajeanett@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/09 15:15:25 by ajeanett          #+#    #+#              #
#    Updated: 2021/07/29 11:22:58 by ajeanett         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
SRCS =	main.cpp \
		CGI.cpp \
		ServerEngine.cpp \
		HTTPBadRequest.cpp \
		HTTPError.cpp \
		HTTPInternalServerError.cpp \
		HTTPNotFound.cpp \
		DeleteResponder.cpp \
		GetResponder.cpp \
		PostResponder.cpp \
		PutResponder.cpp \
		AResponder.cpp \
		Autoindex.cpp \
		Request.cpp \
		Response.cpp \
		LocationData.cpp \
		ParserConfig.cpp \
		ServerData.cpp \
		Date.cpp \
		logging.cpp

OBJ_DIR = obj

OBJ = $(foreach f, $(SRCS), $(addprefix $(OBJ_DIR)/, $(notdir $(f:.cpp=.o))))

FLAGS = clang++ -O2 -I./includes -I./srcs -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	clang++ $(OBJ) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: srcs/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/CGI/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/Engine/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Errors/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Responders/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Responders/Delete/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Responders/Get/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Responders/Post/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Responders/Put/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/Parse/%.cpp
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/Utils/%.cpp
	$(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re %.o
