
/*
 * File: String.hpp
 * Created Data: 2021-6-1
 * Author: tigeroses
 */

#pragma once

// String utilities that process std::string

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
void internalSplit(const std::string& str, 
    std::vector<OutputType>& out, Delim delim, bool skip_empty)
{
    const size_t dSize = delimSize(delim);
    std::string::size_type pos1, pos2;

    pos2 = str.find(delim);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        auto tmp = str.substr(pos1, pos2 - pos1);
        if (!skip_empty || !tmp.empty())
            out.push_back(tmp);

        pos1 = pos2 + dSize;
        pos2 = str.find(delim, pos1);
    }
    auto tmp = str.substr(pos1);
    if (!skip_empty || !tmp.empty())
        out.push_back(tmp);
}

} // namespace detail

template <class Delim, class OutputType>
void split(
    const std::string& input, 
    std::vector<OutputType>& out, 
    const Delim& delim=' ', 
    bool skipEmpty=true)
{
    detail::internalSplit<OutputType>(
        input,
        out,
        detail::prepareDelim(delim),  
        skipEmpty);
}

} // namespace libx


