#include "LocationData.hpp"


LocationData::LocationData(){
	this->_autoindex = false;
	_client_buffer_body_size = 0;
}
LocationData::LocationData(const LocationData &cpy) {
	*this = cpy;
	}
LocationData::~LocationData() {}

LocationData&	LocationData::operator=(const LocationData &cpy) {
	this->_location_path = cpy._location_path;
	this->_full_path = cpy._full_path;
	this->_root = cpy._root;
	this->_cgi_extension = cpy._cgi_extension;
	this->_cgi_path = cpy._cgi_path;
	this->_index = cpy._index;
	this->_redirect = cpy._redirect;
	this->_autoindex = cpy._autoindex;
	this->_methods = cpy._methods;
	this->_client_buffer_body_size = cpy._client_buffer_body_size;
	
	return (*this);
}
size_t	LocationData::getClientBufferBodySize() const{
	return (_client_buffer_body_size);
	};
std::string const			&LocationData::getLocationPath(void) const{
	return (this->_location_path);
}
std::string const			&LocationData::getRoot(void) const {
	return (this->_root);
}
const std::string				&LocationData::getFullPath(void) const {
	return (this->_full_path);
}
const std::string				&LocationData::getCgiPath(void) const {
	return (this->_cgi_path);
}
const std::string				&LocationData::getCgiExtension(void) const {
	return (this->_cgi_extension);
}
const std::string				&LocationData::getIndex(void) const {
	return (this->_index);
}
const std::string				&LocationData::getRedirect(void) const {
	return (this->_redirect);
}
const std::vector<std::string>	&LocationData::getMethods(void) const {
	return (this->_methods);
}

bool					LocationData::getAutoindex() const {
	return (this->_autoindex);
}

// 	void set_location_path(std::string _location_path)
// 	{
// 		this._location_path = _location_path;
// 	}

void					LocationData::setLocationPath(std:: string location){
		this->_location_path = location;
}
void					LocationData::setRoot(std:: string root){
		this->_root = root;
}
void					LocationData::setFullPath(std:: string s1, std:: string s2){
		this->_full_path = s1 + s2;
		size_t _find;
		_find	= _full_path.find("//");
		if (_find != std::string::npos)
		{
			_full_path.erase(_find, 1); // обрезаем лишний слэш
		}
}
void					LocationData::setCgiPath(std:: string cgipath){
		this->_cgi_path = cgipath;
}
void					LocationData::setCgiExtension(std:: string cgiextension){
		this->_cgi_extension = cgiextension;
}
void					LocationData::setIndex(std:: string index){
		this->_index= index;
}
void					LocationData::setRedirect(std:: string redirect){
		this->_redirect = redirect;
}
void					LocationData::setClientBufferBodySize(size_t body_size) {
		this->_client_buffer_body_size = body_size;
}

void					LocationData::setMethods(std::string methods){
	// std::string sentence = "Hello how are you";
	std::istringstream iss(methods);
	// std::vector<std::string> tokens;
	std::copy(std::istream_iterator<std::string>(iss),
          std::istream_iterator<std::string>(),
          std::back_inserter<std::vector<std::string> >(_methods));
	// this->_methods.insert();

		// this->_methods = methods;
}
void					LocationData::setAutoindex(bool autoindex){
		this->_autoindex = autoindex;
}