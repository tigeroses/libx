/*
 * File: System.hpp
 * Created Data: 2021-7-13
 * Author: tigeroses
 */

#pragma once

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <string>

namespace libx {

// Executing shell script using popen() and pipe.
// Note: can only get stdout from pipe, if you want to get stderr,
// add " 2>&1" to the back of command.
// https://linux.die.net/man/3/popen
int subprocess(const std::string& cmd, std::string& out)
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

} // namespace libx