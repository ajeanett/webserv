#ifndef LOCATIONDATA_HPP
#define LOCATIONDATA_HPP
#include <iostream>
#include <vector>
#include <set>
#include <sstream>

class LocationData //данные содержащиеся в локейшене сервера (у сервера может быть несколько локейшенов)
{

private:

	std::string 			_location_path;
	// std::string 			_path;
	std::string 			_root;
	std::string 			_full_path;
	std::string				_cgi_path;
	std::string				_cgi_extension;
	std::string 			_index;
	std::string 			_redirect;
	std::vector<std::string>	_methods;
	size_t					_client_buffer_body_size; // Сравниваем размер тела из запроса для ответа. Если тело в запросе request больше, чем 100 символов, то в ответе возвращается ошибка - посмотреть номер в http
	bool					_autoindex;

public:
	LocationData();
	~LocationData();
	LocationData(const LocationData &copy);
	void					setAutoindex(bool autoindex);
	bool					&getAutoindex();
	void					setLocationPath(std:: string location);
	std::string				&getLocationPath(void);
	void					setRoot(std:: string root);
	std::string				&getRoot(void);
	void					setFullPath(std:: string s1, std:: string s2); //root + location
	std::string				&getFullPath(void);
	void					setCgiPath(std:: string cgipath);
	std::string				&getCgiPath(void);
	void					setCgiExtension(std:: string cgiextension);
	std::string				&getCgiExtension(void);
	void					setIndex(std:: string index);
	std::string				&getIndex();
	void					setRedirect(std::string redirect);
	std::string				&getRedirect(void);
	void					setMethods(std::string methods);
	std::vector<std::string>	&getMethods(void);
	void					setClientBufferBodySize(size_t body_size);
	size_t					&getClientBufferBodySize(void);

//////////////////////////////////////////////
	// size_t&	getBodySize();
	// std::vector<std::string>& getMethods(void);
	// std::string& getRedirect();
	LocationData	&operator=(const LocationData &copy);
};

#endif