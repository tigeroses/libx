/*
 * File: System.hpp
 * Created Data: 2021-7-13
 * Author: tigeroses
 */

#pragma once

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>
#include <random>
#include <filesystem>
namespace fs = std::filesystem;

namespace libx
{

// Executing shell script using popen() and pipe.
// Note: can only get stdout from pipe, if you want to get stderr,
// add " 2>&1" to the back of command.
// https://linux.die.net/man/3/popen
inline int subprocess(const std::string& cmd, std::string& out)
{
    FILE* pp = popen(cmd.c_str(), "r");  // make pipe
    if (!pp)
    {
        out.append(strerror(errno));
        return -1;
    }

    char buff[1024];  // store the stdout per line
    memset(buff, 0, sizeof(buff));
    while (fgets(buff, sizeof(buff), pp) != NULL)
        out.append(buff);

    // close pipe, the return code is cmd's status
    // returns the exit status of the terminating command processor
    // -1 if an error occurs
    int rtn = pclose(pp);

    return rtn;
}

inline int subprocess(const std::string& cmd)
{
    std::string out;
    return libx::subprocess(cmd, out);
}

// Compare last write time of two files
// Return true if first file is older than second file
inline bool compareFileTime(const fs::path& firstFile, const fs::path& secondFile)
{
    auto     t1 = fs::last_write_time(firstFile);
    auto     t2 = fs::last_write_time(secondFile);
    return (std::chrono::duration_cast< std::chrono::milliseconds >(t1 - t2).count() <= 0);
}

inline bool compareFileTime(const std::string firstFile, const std::string secondFile)
{
    fs::path p1(firstFile), p2(secondFile);
    return compareFileTime(p1, p2);
}

// Generate random string include "0-9A-Za-z"
inline std::string genRandomString(int len)
{
    std::string res;

    std::random_device         rd;
    std::default_random_engine random(rd());
    for (int i = 0; i < len; i++)
    {
        char tmp = random() % 62;
        if (tmp < 10)
        {
            tmp += '0';
        }
        else if (tmp < 36)
        {
            tmp -= 10;
            tmp += 'A';
        }
        else
        {
            tmp -= 36;
            tmp += 'a';
        }
        res += tmp;
    }
    return res;
}

}  // namespace libx