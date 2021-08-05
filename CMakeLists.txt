cmake_minimum_required(VERSION 3.19)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_options(-O2 -Wall -std=c++98)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_options(-O2 -Wall -Wextra -Werror -std=c++98)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

include_directories(srcs)
include_directories(includes)

add_executable(webserv
		srcs/main.cpp
		srcs/Engine/ServerEngine.cpp
		srcs/Parse/LocationData.cpp
		srcs/Parse/ServerData.cpp
		srcs/Parse/ParserConfig.cpp
		srcs/Utils/Date.cpp
		srcs/CGI/CGI.cpp
		srcs/CGI/CGI.hpp
		srcs/HTTP/Request.cpp
		srcs/HTTP/Request.hpp
		srcs/HTTP/Response.cpp
		srcs/HTTP/Response.hpp
		srcs/HTTP/Responders/Get/GetResponder.cpp
		srcs/HTTP/Responders/Get/GetResponder.hpp
		srcs/HTTP/Errors/HTTPError.cpp
		srcs/HTTP/Errors/HTTPError.hpp
		srcs/HTTP/Errors/HTTPNotFound.cpp
		srcs/HTTP/Errors/HTTPNotFound.hpp
		srcs/HTTP/Responders/Post/PostResponder.cpp
		srcs/HTTP/Responders/Post/PostResponder.hpp
		includes/logging.hpp
		srcs/Utils/logging.cpp
		srcs/HTTP/Responders/Put/PutResponder.cpp
		srcs/HTTP/Responders/Put/PutResponder.hpp
		srcs/HTTP/Errors/HTTPBadRequest.cpp
		srcs/HTTP/Errors/HTTPBadRequest.hpp
		srcs/HTTP/Autoindex.cpp
		srcs/HTTP/Autoindex.h
		srcs/HTTP/Responders/AResponder.cpp
		srcs/HTTP/Responders/AResponder.hpp
		srcs/HTTP/Errors/HTTPInternalServerError.cpp
		srcs/HTTP/Errors/HTTPInternalServerError.hpp
		srcs/HTTP/Request_data.hpp
		srcs/HTTP/Responders/Delete/DeleteResponder.cpp
		srcs/HTTP/Responders/Delete/DeleteResponder.hpp
)
