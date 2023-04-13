#ifndef UTILS_HPP
#define UTILS_HPP
# pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <stdlib.h> 
#include "macro.hpp"

std::string              itostr(int i);
std::string              shrinkNBlank(std::string const &str, size_t n = std::string::npos);
std::vector<std::string> ke_split(std::string const & str, std::string const & pattern);
std::vector<std::string> ke_nSplit(std::string const & str, std::string const & pattern, size_t maxEl);
bool                     isPrintableMsg(std::string const & str);
bool					 checkArg(std::string const & str);

#endif
