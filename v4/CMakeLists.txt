cmake_minimum_required(VERSION 3.19)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

#add_compile_options(-Wall -Wextra -Werror -std=c++98)
add_compile_options(-Wall -std=c++98)

#include_directories(srcs)

add_executable(webserv
		main.cpp
		server_engine/ServerEngine.cpp
		parse_config/LocationData.cpp
		parse_config/ServerData.cpp
		parse_config/ParserConfig.cpp
		server_engine/Date.cpp
		CGI/CGI.cpp
		CGI/CGI.hpp
		HTTP/Request.cpp
		HTTP/Request.hpp
		HTTP/Response.cpp
		HTTP/Response.hpp
)
