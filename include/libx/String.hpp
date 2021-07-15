
/*
 * File: String.hpp
 * Created Data: 2021-6-1
 * Author: tigeroses
 */

// String utilities that process std::string.
//

#pragma once

#include <libx/Conv.hpp>

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace libx
{

namespace detail
{

    inline std::string prepareDelim(const std::string& s)
    {
        return s;
    }
    inline char prepareDelim(char c)
    {
        return c;
    }

    inline size_t delimSize(const char&)
    {
        return 1;
    }
    inline size_t delimSize(const std::string& s)
    {
        return s.size();
    }

    template < class OutputType, class Delim >
    void internalSplit(const std::string& str, const Delim& delim,
                       std::vector< OutputType >& out, bool skip_empty)
    {
        const size_t           dSize = delimSize(delim);
        std::string::size_type pos1, pos2;

        pos2 = str.find(delim);
        pos1 = 0;
        while (std::string::npos != pos2)
        {
            auto tmp = str.substr(pos1, pos2 - pos1);
            if (!skip_empty || !tmp.empty())
                out.push_back(to< OutputType >(tmp));

            pos1 = pos2 + dSize;
            pos2 = str.find(delim, pos1);
        }
        auto tmp = str.substr(pos1);
        if (!skip_empty || !tmp.empty())
            out.push_back(to< OutputType >(tmp));
    }

    template < class Delim, class OutputType >
    bool splitFixed(const std::string& str, const Delim& delim,
                    OutputType& output)
    {
        if (std::string::npos != str.find(delim))
            return false;

        output = libx::to< OutputType >(str);
        return true;
    }

    template < class Delim, class OutputType, class... OutputTypes >
    bool splitFixed(const std::string& str, const Delim& delim,
                    OutputType& outHead, OutputTypes&... output)
    {
        size_t cut = str.find(delim);
        if (cut == std::string::npos)
            return false;

        std::string head(str.begin(), str.begin() + cut);
        std::string tail(str.begin() + cut + delimSize(delim), str.end());
        if (splitFixed(tail, delim, output...))
        {
            outHead = libx::to< OutputType >(head);
            return true;
        }
        return false;
    }

    inline void appendDelim(const char& delim, std::string& out)
    {
        out.append(1, delim);
    }

    inline void appendDelim(const std::string& delim, std::string& out)
    {
        out.append(delim);
    }

    template < class Iterator, class Delim >
    void internalJoin(Iterator begin, Iterator end, const Delim& delim,
                      std::string& out)
    {
        if (begin == end)
            return;

        auto iter = begin;
        out.append(toStr(*iter));
        while (++iter != end)
        {
            appendDelim(delim, out);
            out.append(toStr(*iter));
        }
    }

    template < class Delim, class InputType >
    void joinFixed(const Delim& delim, std::string& out, InputType& head)
    {
        out.append(toStr(head));
    }

    template < class Delim, class InputType, class... InputTypes >
    void joinFixed(const Delim& delim, std::string& out, InputType& head,
                   InputTypes&... inputs)
    {
        out.append(toStr(head));
        appendDelim(delim, out);

        joinFixed(delim, out, inputs...);
    }

    static inline bool isOddSpace(const char c)
    {
        return (c == '\t' || c == '\n' || c == '\r');
    }

}  // namespace detail

// There are two methods for splitting a string into a list of tokens by
// delimiter.
//
// The first interface supports different output types, selected at compile
// time: std::string, int, float, double, etc. It detects the type based on what
// your vector contains. If the output vector is not empty, the split will
// append to the end of the vector. Examples:
//
//      std::vector<std::string> v;
//      libx::split("a:bc:def", ':', v);
//
//      std::vector<int> v;
//      libx::split("1\t23\t456\t", '\t', v);
//
// Split also takes a flag (skipEmpty) that indicates whether adjacent
// delimiters should be treated as one single separator or not.
//
// The second interface split a string into a fixed number of string pieces
// and/or numeric types by delimiter. Conversions are supported for any type
// which libx::to<> can target. Returns 'true' if the fields were all
// successfully populated. Casting exceptions will not be caught. Examples:
//
//      std::string a;
//      int b;
//      double c;
//      if (libx::split("abc 123 45.678", " ", a, b, c))
//          ...

template < class Delim, class OutputType >
void split(const std::string& input, const Delim& delim,
           std::vector< OutputType >& out, bool skipEmpty = true)
{
    detail::internalSplit< OutputType >(input, detail::prepareDelim(delim), out,
                                        skipEmpty);
}

template < class Delim, class... OutputTypes >
bool split(const std::string& input, const Delim& delim = ' ',
           OutputTypes&... outputs)
{
    return detail::splitFixed(input, detail::prepareDelim(delim), outputs...);
}

// There are three methods for concatenating a list of tokens by delimiter,
// and they all support return a string and pass string as parameter.
//
// 1. vector<> as input
//
//      vector<int> vec(1,2,3);
//      cout<<libx::join(vec, ' ');
//
// 2. initializer_list<> as input
//
//      cout<<libx::join({"a", "bc", "def"}, "\t");
//
// 3. variadic template
//
//      string out;
//      int i = 1;
//      float f = 2.55;
//      string s = "libx";
//      libx::join('-', out, i, f, s);
//      if (out == "1-2.550000-libx")
//          ...
//

template < class Delim, class InputType >
void join(const std::vector< InputType >& input, const Delim& delim,
          std::string& out)
{
    detail::internalJoin(input.begin(), input.end(),
                         detail::prepareDelim(delim), out);
}

template < class Delim, class InputType >
std::string join(const std::vector< InputType >& input, const Delim& delim)
{
    std::string out;
    detail::internalJoin(input.begin(), input.end(),
                         detail::prepareDelim(delim), out);
    return out;
}

template < class Delim, class InputType >
void join(const std::initializer_list< InputType >& input, const Delim& delim,
          std::string& out)
{
    detail::internalJoin(input.begin(), input.end(),
                         detail::prepareDelim(delim), out);
}

template < class Delim, class InputType >
std::string join(const std::initializer_list< InputType >& input,
                 const Delim&                              delim)
{
    std::string out;
    detail::internalJoin(input.begin(), input.end(),
                         detail::prepareDelim(delim), out);
    return out;
}

template < class Delim, class... InputTypes >
void join(const Delim& delim, std::string& out, InputTypes&... input)
{
    detail::joinFixed(detail::prepareDelim(delim), out, input...);
}

template < class Delim, class... InputTypes >
std::string join(const Delim& delim, InputTypes&... input)
{
    std::string out;
    detail::joinFixed(detail::prepareDelim(delim), out, input...);
    return out;
}

// Remove substring from the front or back of string
// for whitespace, means any of [' ', '\n', '\t', '\r']
// for other characters, the user should provide a function or lambda expression
// to check how to trim string, for example:
//
//      auto toTrim = [](const char c) { return (c == 'a' || c == 'b') };

// Returns a substring with all whitespace removed from the front of string
inline std::string ltrimWhitespace(const std::string& s)
{
    std::string s_(s);
    while (true)
    {
        while (!s.empty() && s_.front() == ' ')
            s_.erase(s_.begin());
        if (detail::isOddSpace(s_.front()))
        {
            s_.erase(s_.begin());
            continue;
        }
        return s_;
    }
}

// Returns a substring with all whitespace removed from the back of string
inline std::string rtrimWhitespace(const std::string& s)
{
    std::string s_(s);
    while (true)
    {
        while (!s.empty() && s_.back() == ' ')
            s_.pop_back();
        if (detail::isOddSpace(s_.back()))
        {
            s_.pop_back();
            continue;
        }
        return s_;
    }
}

// Returns a substring with all whitespace removed from the front and back of
// string
inline std::string trimWhitespace(const std::string& s)
{
    return rtrimWhitespace(ltrimWhitespace(s));
}

// Returns a substring with all characters the ToTrim returns true removed from
// the front of string
template < typename ToTrim >
std::string ltrim(const std::string& s, ToTrim toTrim)
{
    std::string s_(s);
    while (!s_.empty() && toTrim(s_.front()))
    {
        s_.erase(s_.begin());
    }
    return s_;
}

// Returns a substring with all characters the ToTrim returns true removed from
// the back of string
template < typename ToTrim >
std::string rtrim(const std::string& s, ToTrim toTrim)
{
    std::string s_(s);
    while (!s_.empty() && toTrim(s_.back()))
    {
        s_.pop_back();
    }
    return s_;
}

// Returns a substring with all characters the ToTrim returns true removed from
// the front and back of string
template < typename ToTrim >
std::string trim(const std::string& s, ToTrim toTrim)
{
    return rtrim(ltrim(s, std::ref(toTrim)), std::ref(toTrim));
}

// Check if a string starts with another string
inline bool startswith(const std::string& s, const std::string& prefix)
{
    if (s.empty() || prefix.empty() || s.size() < prefix.size()) return false;

    for (size_t i = 0; i < prefix.size(); ++i)
    {
        if (s.at(i) != prefix.at(i))
            return false;
    }
    return true;
}

// Check if a string ends with another string
inline bool endswith(const std::string& s, const std::string& suffix)
{
    if (s.empty() || suffix.empty() || s.size() < suffix.size()) return false;

    size_t start = s.size() - suffix.size();
    for (size_t i = 0; i < suffix.size(); ++i)
    {
        if (s.at(i+start) != suffix.at(i))
            return false;
    }
    return true;
}

}  // namespace libx
