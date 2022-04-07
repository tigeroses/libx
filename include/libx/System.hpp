/*
 * File: System.hpp
 * Created Data: 2021-7-13
 * Author: tigeroses
 */

#pragma once

#ifdef _WIN32
#include <direct.h>
#define popen _popen
#define pclose _pclose
#else
#include <sys/sysinfo.h>
#include <unistd.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>
#include <random>
#include <filesystem>
namespace fs = std::filesystem;

#include "libx/File.hpp"

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

// Get system memory by reads system file '/proc/meminfo',
// the format of first line: "MemTotal:       32263508 kB"
// the memory unit is GB,
// return negative value when something wrong, positive value
// as system memory
inline int getSystemMemory()
{
    // set default memory as 64GB
    int    res = -1;
    std::string filename("/proc/meminfo");
    auto   parseTotalMemory = [&](std::string& line) {
        std::vector< std::string > vec;
        libx::split(line, ' ', vec);
        try
        {
            if (libx::startswith(line, "MemTotal") && vec.size() == 3)
                res = libx::to< long >(vec[1]) / 1024 / 1024;
        }
        catch (...)
        {
            res = -1;
        }
        return false;
    };
    libx::readFile(filename, parseTotalMemory);
    return res;
}

// Get memory usage of Process by read system file '/proc/$PID/stat'
// Parameters:
//  pid: default -1 for current Process, else Process ID
//  unit: default BYTE, support "BYTE" "KB" "MB" "GB"
inline void getProcessMemory(double& virtualMem, double& physicalMem, std::string unit="BYTE", int processID = -1)
{
    virtualMem     = 0.0;
    physicalMem = 0.0;

    // 'file' stat seems to give the most reliable results
    std::string filename = (processID < 0) ? "/proc/self/stat" : "/proc/"+std::to_string(processID)+"/stat";
    std::ifstream stat_stream(filename, std::ios_base::in);

    // dummy vars for leading entries in stat that we don't care about
    std::string pid, comm, state, ppid, pgrp, session, tty_nr;
    std::string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    std::string utime, stime, cutime, cstime, priority, nice;
    std::string O, itrealvalue, starttime;

    // the two fields we want
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                >> utime >> stime >> cutime >> cstime >> priority >> nice
                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    virtualMem     = vsize / 1024.0;
    physicalMem = rss * page_size_kb;

    // unit conversion
    int div = 1.0;
    if (unit.empty()) 
        unit = "BYTE";
    std::transform(unit.begin(), unit.end(), unit.begin(), ::toupper);
    if (unit[0] == 'K')
        div *= 1024;
    if (unit[0] == 'M')
        div *= 1024*1024;
    if (unit[0] == 'G')
        div *= 1024*1024*1024;
    
    virtualMem /= div;
    physicalMem /= div;
}

// Get physical memory of current Process, in GB unit
inline int getSelfMemory()
{
    double vm, pm;
    std::string unit("GB");
    int pid = -1;
    getProcessMemory(vm, pm, unit, pid);
    return pm;
}

}  // namespace libx