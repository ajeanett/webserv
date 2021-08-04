//
// Created by Antioch Jeanett on 8/4/21.
//

#ifndef WEBSERV_REQUEST_DATA_HPP
#define WEBSERV_REQUEST_DATA_HPP

#include <iostream>

class t_req_data
{
public:
	std::string buffer;
	bool _is_transfer_encoding;
	bool _is_content_length;
	bool _is_not_body;
	size_t _body_size;
	bool _is_body; //\r\n\r\n
	size_t _start_body;
	size_t _cur_pos;
	t_req_data() {
		this->clear();
	}
	void clear() {
		_is_transfer_encoding = false;
		_is_content_length = false;
		_is_not_body = false;
		_is_body = false;
		_body_size = 0;
		_start_body = 0;
		_cur_pos = 0;
		buffer.clear();
	}
};

#endif //WEBSERV_REQUEST_DATA_HPP
