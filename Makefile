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

HEADERS =	includes/colors.hpp includes/logging.hpp \
			srcs/CGI/CGI.hpp \
			srcs/ENGINE/ServerEngine.hpp \
			srcs/HTTP/Autoindex.h srcs/HTTP/Request.hpp srcs/HTTP/Request_data.hpp srcs/HTTP/Response.hpp \
			srcs/HTTP/Errors/HTTPBadRequest.hpp srcs/HTTP/Errors/HTTPError.hpp srcs/HTTP/Errors/HTTPInternalServerError.hpp srcs/HTTP/Errors/HTTPNotFound.hpp \
			srcs/HTTP/Responders/AResponder.hpp srcs/HTTP/Responders/Delete/DeleteResponder.hpp srcs/HTTP/Responders/Get/GetResponder.hpp srcs/HTTP/Responders/Post/PostResponder.hpp srcs/HTTP/Responders/Put/PutResponder.hpp \
			srcs/Parse/LocationData.hpp srcs/Parse/ParserConfig.hpp srcs/Parse/ServerData.hpp \
			srcs/Utils/Date.hpp

OBJ_DIR = obj

OBJ = $(foreach f, $(SRCS), $(addprefix $(OBJ_DIR)/, $(notdir $(f:.cpp=.o))))

FLAGS = clang++ -O2 -I./includes -I./srcs -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ)
	clang++ $(OBJ) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: srcs/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/CGI/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/Engine/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Errors/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Responders/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Responders/Delete/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Responders/Get/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Responders/Post/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/HTTP/Responders/Put/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/Parse/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: srcs/Utils/%.cpp $(HEADERS)
	$(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re %.o
