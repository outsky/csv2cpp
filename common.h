#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>

std::vector<std::string> str_split(std::string str, int n=-1,
        char tok=' ');

void str_trim(std::string& str, const std::string& tok);

#endif
