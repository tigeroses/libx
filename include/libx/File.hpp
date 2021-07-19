/*
 * File: File.hpp
 * Created Data: 2021-7-15
 * Author: tigeroses
 */

#pragma once

#include "libx/String.hpp"

#include <string>
#include <fstream>
#include <vector>

namespace libx {

// Read and process data from file
// filename: input filename
// processLine: a lambda function for processing one line in file
// skipPrefix: the comment type to be skipped, default "#"
// skipLineNum: if there is not exists skipPrefix, you should specify the line number to be skipped
// Return false if there is not exists the file, it is also skip empty lines in file
template < typename ProcessLine >
inline bool readFile(const std::string& filename, ProcessLine processLine,
    const std::string& skipPrefix="#", int skipLineNum=0)
{
    std::ifstream ifs(filename);
    if (ifs.fail()) return false;

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

    ifs.close();
    return true;
}

// Write string to file
inline bool writeFile(const std::string& str, const std::string& filename,
    bool append = false)
{
    std::ofstream ofs;
    if (append)
        ofs.open(filename, std::ofstream::app);
    else
        ofs.open(filename);
    if (ofs.fail()) return false;

    ofs << str;

    ofs.close();
    return true;
}

// Write list of data to file
template< class InputType >
inline bool writeFile(const std::vector<InputType>& records, const std::string& filename,
    bool append = false)
{
    std::ofstream ofs;
    if (append)
        ofs.open(filename, std::ofstream::app);
    else
        ofs.open(filename);
    if (ofs.fail()) return false;

    for (auto& record : records)
        ofs << record << std::endl;

    ofs.close();
    return true;
}

} // namespace libx