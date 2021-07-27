#include "date.hpp"

std::string Date::get_time(){

    gettimeofday(&_t, NULL);
    _tm = gmtime(&_t.tv_sec);
    strftime(_buf, 1000, "%A %Y %H:%M:%S", _tm);

    _ret = _buf;

    return (_ret);

}































//"%a, %d %b %Y %H:%M:%S GMT"