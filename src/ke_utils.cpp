#include "ke_utils.hpp"

std::vector<std::string>    ke_split(std::string const & str, std::string const & pattern) {
    std::vector<std::string> vec;
    
    size_t index = 0;
    while (str[index]) {

        size_t end = str.find(pattern, index);
        std::string el = str.substr(index, end - index);
        index = end + 2;
        vec.push_back(el);
    }
    return vec;
}


std::string shrinkNBlank(std::string const & str, size_t n /* = std::string::npos*/) {

    std::string s;
    size_t sp_i = 0;
    size_t ch_i   = 0;

    if (str.at(0) == ' ' || str.at(str.length() - 1) == ' ')
        throw std::invalid_argument("Leading or trailing blank space");
    
    while (n--) {
        sp_i = str.find(' ', ch_i);
        if (sp_i == std::string::npos) break;
        s += str.substr(ch_i, sp_i - ch_i + 1);
        ch_i = str.find_first_not_of(' ', sp_i);
    }
    s += str.substr(ch_i, std::string::npos);

    return s;
}