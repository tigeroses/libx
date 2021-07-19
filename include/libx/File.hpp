/*
 * File: File.hpp
 * Created Data: 2021-7-15
 * Author: tigeroses
 */

#pragma once

#include "libx/String.hpp"

#include <string>
#include <fstream>

namespace libx {

// Read and process data from file
// filename: input filename
// processLine: a lambda function for processing one line in file
// skipPrefix: the comment type to be skipped, default "#"
// skipLineNum: if there is not exists skipPrefix, you should specify the line number to be skipped
// It is also skip empty lines in file
template < typename ProcessLine >
inline void readFile(const std::string& filename, ProcessLine processLine,
    const std::string& skipPrefix="#", int skipLineNum=0)
{
    std::ifstream ifs(filename);
    std::string line;
    // Skip header lines and comment lines
    while (skipLineNum-- > 0)
        std::getline(ifs, line);
    while (std::getline(ifs, line))
        if (!line.empty() && !libx::startswith(line, skipPrefix))
            break;
    
    do
    {
        if (line.empty()) continue;
        processLine(line);
    } while (std::getline(ifs, line));
}

} // namespace libx