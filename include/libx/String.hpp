
/*
 * File: String.hpp
 * Created Data: 2021-6-1
 * Author: tigeroses
 */

// String utilities that process std::string.
// 
// There are two methods for splitting a string into a list of tokens by delimiter.
//
// The first interface supports different output types, selected at compile time:
// std::string, int, float, double, etc. It detects the type based on what your
// vector contains. If the output vector is not empty, the split will append to the
// end of the vector. Examples:
//
//      std::vector<std::string> v;
//      libx::split("a:bc:def", ':', v);
//
//      std::vector<int> v;
//      libx::split("1\t23\t456\t", '\t', v);
//
// Split also takes a flag (skipEmpty) that indicates whether adjacent delimiters
// should be treated as one single separator or not.
//
// The second interface split a string into a fixed number of string pieces
// and/or numeric types by delimiter. Conversions are supported for any type
// which libx::to<> can target. Returns 'true' if the fields were all successfully
// populated. Casting exceptions will not be caught. Examples:
//
//      std::string a;
//      int b;
//      double c;
//      if (libx::split("abc 123 45.678", " ", a, b, c))
//          ...

#pragma once

#include <libx/Conv.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace libx {

namespace detail {

inline std::string prepareDelim(const std::string& s)
{
    return s;
}
inline char prepareDelim(char c)
{
    return c;
}

inline size_t delimSize(char)
{
    return 1;
}
inline size_t delimSize(std::string& s)
{
    return s.size();
}

template <class OutputType, class Delim>
void internalSplit(const std::string& str, Delim delim,
    std::vector<OutputType>& out, bool skip_empty)
{
    const size_t dSize = delimSize(delim);
    std::string::size_type pos1, pos2;

    pos2 = str.find(delim);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        auto tmp = str.substr(pos1, pos2 - pos1);
        if (!skip_empty || !tmp.empty())
            out.push_back(to<OutputType>(tmp));

        pos1 = pos2 + dSize;
        pos2 = str.find(delim, pos1);
    }
    auto tmp = str.substr(pos1);
    if (!skip_empty || !tmp.empty())
        out.push_back(to<OutputType>(tmp));
}

template<class Delim, class OutputType>
bool splitFixed(const std::string& str,
    const Delim& delim, OutputType& output)
{
    if (std::string::npos != str.find(delim))
        return false;

    output = libx::to<OutputType>(str);
    return true;
}

template<class Delim, class OutputType, class... OutputTypes>
bool splitFixed(const std::string& str, 
    const Delim& delim, OutputType& outHead, OutputTypes&... output)
{
    size_t cut = str.find(delim);
    if (cut == std::string::npos)
        return false;
    
    std::string head(str.begin(), str.begin()+cut);
    std::string tail(str.begin() + cut + delimSize(delim), str.end());
    if (splitFixed(tail, delim, output...))
    {
        outHead = libx::to<OutputType>(head);
        return true;
    }
    return false;
}

} // namespace detail

template <class Delim, class OutputType>
void split(
    const std::string& input,
    const Delim& delim, 
    std::vector<OutputType>& out, 
    bool skipEmpty=true)
{
    detail::internalSplit<OutputType>(
        input,
        detail::prepareDelim(delim),  
        out,
        skipEmpty);
}

template<class Delim, class... OutputTypes>
bool split(
    const std::string& input,
    const Delim& delim=' ',
    OutputTypes&... outputs)
{
    return detail::splitFixed(
        input,
        detail::prepareDelim(delim),
        outputs...);
}

} // namespace libx


