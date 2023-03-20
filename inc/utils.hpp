#ifndef UTILS_HPP
#define UTILS_HPP
# pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

std::string              itostr(int i);
std::string              shrinkNBlank(std::string const &str, size_t n = std::string::npos);
std::vector<std::string> ke_split(std::string const & str, std::string const & pattern);

#endif
