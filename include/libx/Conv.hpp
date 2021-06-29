
/*
 * File: Conv.hpp
 * Created Data: 2021-6-4
 * Author: tigeroses
 */

/*
 * This file provides a generic interface for converting objects from
 * std::string to numeric types.
 *
 * Parsing a string into a C++ numeric type, for example,
 * to<int>("123"); // returns 123
 */

#pragma once

#include <string>

namespace libx
{

// Convert numerical types to std::string
// Warning: keep in mind there is six precision for float type
template < class SrcType >
std::string toStr(SrcType src)
{
    return std::to_string(src);
}

template <>
inline std::string toStr(std::string src)
{
    return src;
}

template <>
inline std::string toStr(const char* src)
{
    return std::string(src);
}

// Convert std::string to numerical types
template < class TgtType >
TgtType to(const std::string& src)
{
    return TgtType();
}

template <>
inline std::string to(const std::string& src)
{
    return src;
}

template <>
inline int to(const std::string& src)
{
    return std::stoi(src);
}

template <>
inline float to(const std::string& src)
{
    return std::stof(src);
}

template <>
inline double to(const std::string& src)
{
    return std::stod(src);
}

template <>
inline long to(const std::string& src)
{
    return std::stol(src);
}

template <>
inline long double to(const std::string& src)
{
    return std::stold(src);
}

template <>
inline long long to(const std::string& src)
{
    return std::stoll(src);
}

template <>
inline unsigned long to(const std::string& src)
{
    return std::stoul(src);
}

template <>
inline unsigned long long to(const std::string& src)
{
    return std::stoull(src);
}

}  // namespace libx