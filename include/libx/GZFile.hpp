/*
 * File: GZFile.hpp
 * Created Data: 2021-7-20
 * Author: tigeroses
 */

#pragma once

#include <libx/String.hpp>

#include <string>
#include <iostream>

// for using strerror
#include <string.h>
#include <errno.h>

#include <zlib.h>

namespace libx {

namespace detail {

static constexpr int GZ_LINE_LEN = 1024;
static char GZ_LINE_BUFF[GZ_LINE_LEN];

bool getline(gzFile f, std::string& line)
{
    if (gzgets(f, GZ_LINE_BUFF, GZ_LINE_LEN) == Z_NULL)
    {
        // end-of-file or error
        int         err;
        const char* msg = gzerror(f, &err);
        if (err != Z_OK)
        {
            std::cerr << "read gz file error, error_code: " << err << " error_msg: " << msg << std::endl;
        }
        // cout<<"eof"<<endl;
        return false;
    }
    
    line.assign(GZ_LINE_BUFF);
    // remove \n
    if (line.back() == '\n')
        line.pop_back();
        
    return true;
}

} // namespace detail

// Read and write .gz file
// Compile code with "-lz"
template < typename ProcessLine >
inline bool readGZFile(const std::string& filename, ProcessLine processLine,
    const std::string& skipPrefix="#", int skipLineNum=0)
{
    gzFile fh = gzopen(filename.c_str(), "rb");
    if (fh == Z_NULL)
    {
        std::cerr<<"read gz file "<<filename<<" failed, error: "<<strerror(errno)<<std::endl;
        return false;
    }

    std::string line;
    // Skip header lines and comment lines
    while (skipLineNum-- > 0)
        detail::getline(fh, line);
    while (detail::getline(fh, line))
        if (!line.empty() && !libx::startswith(line, skipPrefix))
            break;
    
    do
    {
        if (line.empty()) continue;
        if (!processLine(line)) break;
    } while (detail::getline(fh, line));

    gzclose(fh);
    return true;
}

} // namespace libx