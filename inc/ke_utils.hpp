#ifndef KE_UTILS_HPP
# define KE_UTILS_HPP
# pragma once

#include <string>
#include <vector>
#include <stdexcept>


std::string              shrinkNBlank(std::string const &str, size_t n = std::string::npos);
std::vector<std::string> ke_split(std::string const & str, std::string const & pattern);

#endif