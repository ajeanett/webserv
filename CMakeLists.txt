cmake_minimum_required(VERSION 3.19)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_options(-Wall -std=c++98)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_options(-Wall -Wextra -Werror -std=c++98)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

include_directories(srcs)
include_directories(includes)

add_executable(webserv
		srcs/main.cpp
		srcs/server_engine/ServerEngine.cpp
		srcs/parse_config/LocationData.cpp
		srcs/parse_config/ServerData.cpp
		srcs/parse_config/ParserConfig.cpp
		srcs/server_engine/Date.cpp
		srcs/CGI/CGI.cpp
		srcs/CGI/CGI.hpp
		srcs/HTTP/Request.cpp
		srcs/HTTP/Request.hpp
		srcs/HTTP/Response.cpp
		srcs/HTTP/Response.hpp
		srcs/HTTP/Responders/GetResponder.cpp
		srcs/HTTP/Responders/GetResponder.hpp
		srcs/HTTP/Responders/IResponder.hpp
		srcs/HTTP/Errors/HTTPError.cpp
		srcs/HTTP/Errors/HTTPError.hpp
		srcs/HTTP/Errors/HTTPNotFound.cpp
		srcs/HTTP/Errors/HTTPNotFound.hpp
		srcs/HTTP/Responders/PostResponder.cpp
		srcs/HTTP/Responders/PostResponder.hpp
		includes/logging.hpp
		srcs/Utils/logging.cpp
)
