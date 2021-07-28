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
	bool					getAutoindex() const;
	void					setLocationPath(std:: string location);
	std::string const		&getLocationPath(void) const;
	void					setRoot(std:: string root);
	const std::string				&getRoot(void) const;
	void					setFullPath(std:: string s1, std:: string s2); //root + location
	const std::string				&getFullPath(void) const;
	void					setCgiPath(std:: string cgipath);
	const std::string				&getCgiPath(void) const;
	void					setCgiExtension(std:: string cgiextension);
	const std::string				&getCgiExtension(void) const;
	void					setIndex(std:: string index);
	const std::string				&getIndex() const;
	void					setRedirect(std::string redirect);
	const std::string				&getRedirect(void) const;
	void					setMethods(std::string methods);
	const std::vector<std::string>	&getMethods(void) const;
	void					setClientBufferBodySize(size_t body_size);
	size_t					getClientBufferBodySize(void) const;

//////////////////////////////////////////////
	// size_t&	getBodySize();
	// std::vector<std::string>& getMethods(void);
	// std::string& getRedirect();
	LocationData	&operator=(const LocationData &copy);
};

#endif