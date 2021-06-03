
/*
 * File: String.hpp
 * Created Data: 2021-6-1
 * Author: tigeroses
 */

// String utilities that process std::string

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace libx {

// std::vector< std::string > split(const std::string& str, char delim=' ', bool skip_empty=true)
// {
//     std::istringstream iss(str);
//     std::vector< std::string >   res;
//     for (std::string item; getline(iss, item, delim);)
//         if (skip_empty && item.empty())
//             continue;
//         else
//             res.push_back(item);
//     return res;
// }

// void split(const std::string& str, std::vector< std::string >& res, char delim=' ', bool skip_empty=true)
// {
//     std::istringstream iss(str);
//     res.clear();
//     for (std::string item; getline(iss, item, delim);)
//         if (skip_empty && item.empty())
//             continue;
//         else
//             res.push_back(item);
// }

namespace detail {

std::string prepareDelim(const std::string& s)
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
    std::cout<<typeid(delim).name()<<std::endl;
    detail::internalSplit<OutputType>(
        input,
        out,
        detail::prepareDelim(delim),  
        skipEmpty);
}

// void split(const std::string& str, std::vector< std::string >& res, char delim=' ', bool skip_empty=true)
// {
//     res.clear();
//     size_t start = 0;
//     size_t len = 0;
//     for (size_t i = 0; i < str.size(); ++i)
//     {
//         if (str[i] == delim)
//         {
            
//             auto tmp = str.substr(start, len);
//             if (!skip_empty || !tmp.empty())
//                 res.push_back(tmp);

//             start = i+1;
//             len = 0;
           
//         }
//         else
//         {
//             ++len;
//         }
//     }

//     auto tmp = str.substr(start, len);
//     if (!skip_empty || !tmp.empty())
//         res.push_back(tmp);
// }



} // namespace libx


