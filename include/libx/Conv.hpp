
/*
 * File: Conv.hpp
 * Created Data: 2021-6-4
 * Author: tigeroses
 */

/*
 * This file provides a generic interface for converting objects from std::string
 * to numeric types.
 * 
 * Parsing a string into a C++ numeric type, for example,
 * to<int>("123"); // returns 123
 */

#pragma once

#include <string>

namespace libx {

template<class TgtType>
TgtType to(const std::string& src)
{
    return TgtType();
}

template<>
inline std::string to(const std::string& src)
{
    return src;
}

template<>
inline int to(const std::string& src)
{
    return std::stoi(src);
}

template<>
inline float to(const std::string& src)
{
    return std::stof(src);
}

template<>
inline double to(const std::string& src)
{
    return std::stod(src);
}

template<>
inline long to(const std::string& src)
{
    return std::stol(src);
}

template<>
inline long double to(const std::string& src)
{
    return std::stold(src);
}

template<>
inline long long to(const std::string& src)
{
    return std::stoll(src);
}

template<>
inline unsigned long to(const std::string& src)
{
    return std::stoul(src);
}

template<>
inline unsigned long long to(const std::string& src)
{
    return std::stoull(src);
}

} // namespace libx