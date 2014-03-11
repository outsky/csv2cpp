#include "common.h"

std::vector<std::string> str_split(std::string str, int n,
        const char tok) {
    std::vector<std::string> ret;

    std::string::size_type i = 0;
    for(unsigned int idx=0; idx<(unsigned int)n; ++idx) {
        i = str.find(tok);
        if(i == std::string::npos) {
            ret.push_back(str);
            break;
        }

        ret.push_back(str.substr(0, i));
        str = str.substr(i+1);
    }

    if(i != std::string::npos)
        ret.push_back(str);

    return ret;
}

void str_trim(std::string& str, const std::string& tok) {
    size_t pos = std::string::npos;
    while( true ) {
        pos = str.find_first_of(tok);
        if(pos == std::string::npos)
            break;
        str.erase(pos, 1);
    }
}
