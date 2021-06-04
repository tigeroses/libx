
/*
 * File: Conv.hpp
 * Created Data: 2021-6-4
 * Author: tigeroses
 */

#pragma once

#include <string>

namespace libx {

template<class TgtType>
TgtType to(const std::string& src)
{
    return TgtType();
}

template<class SrcType>
std::string to(const SrcType& src)
{
    return std::to_string(src);
}

template<>
int to(const std::string& src)
{
    return std::stoi(src);
}

template<>
float to(const std::string& src)
{
    return std::stof(src);
}

template<>
double to(const std::string& src)
{
    return std::stod(src);
}

template<>
long to(const std::string& src)
{
    return std::stol(src);
}

template<>
long double to(const std::string& src)
{
    return std::stold(src);
}

template<>
long long to(const std::string& src)
{
    return std::stoll(src);
}

template<>
unsigned long to(const std::string& src)
{
    return std::stoul(src);
}

template<>
unsigned long long to(const std::string& src)
{
    return std::stoull(src);
}

} // namespace libx