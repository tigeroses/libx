/*
 * File: GZFile.hpp
 * Created Data: 2021-7-20
 * Author: tigeroses
 */

#pragma once

#include <libx/String.hpp>

#include <iostream>
#include <string>

// for using strerror
#include <errno.h>
#include <string.h>

#include <zlib.h>

namespace libx
{

namespace detail
{

    static constexpr int GZ_LINE_LEN = 1024;
    static char          GZ_LINE_BUFF[GZ_LINE_LEN];
    static char          GZ_NEW_LINE[1] = { '\n' };

    inline bool getline(gzFile f, std::string& line)
    {
        if (gzgets(f, GZ_LINE_BUFF, GZ_LINE_LEN) == Z_NULL)
        {
            // end-of-file or error
            int         err;
            const char* msg = gzerror(f, &err);
            if (err != Z_OK)
            {
                std::cerr << "read gz file error, error_code: " << err
                          << " error_msg: " << msg << std::endl;
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

}  // namespace detail

// Read and write .gz file
// Compile code with "-lz"
template < typename ProcessLine >
inline bool readGZFile(const std::string& filename, ProcessLine processLine,
                       const std::string& skipPrefix = "#", int skipLineNum = 0)
{
    gzFile fh = gzopen(filename.c_str(), "rb");
    if (fh == Z_NULL)
    {
        std::cerr << "open gz file " << filename
                  << " failed, error: " << strerror(errno) << std::endl;
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
        if (line.empty())
            continue;
        if (!processLine(line))
            break;
    } while (detail::getline(fh, line));

    gzclose(fh);
    return true;
}

// Write string to gz file
inline bool writeGZFile(const std::string& str, const std::string& filename,
                        bool append = false)
{
    gzFile fh;
    if (append)
        fh = gzopen(filename.c_str(), "ab");
    else
        fh = gzopen(filename.c_str(), "wb");
    if (fh == Z_NULL)
    {
        std::cerr << "open gz file " << filename
                  << " failed, error: " << strerror(errno) << std::endl;
        return false;
    }

    int writeBytes = gzputs(fh, str.c_str());
    if (writeBytes != str.size())
    {
        std::cerr << "write gz file " << filename
                  << " failed, data size: " << str.size()
                  << " write bytes: " << writeBytes << std::endl;
        return false;
    }

    gzclose(fh);
    return true;
}

// Write vector of string to gz file
inline bool writeGZFile(const std::vector< std::string >& records,
                        const std::string& filename, bool append = false)
{
    gzFile fh;
    if (append)
        fh = gzopen(filename.c_str(), "ab");
    else
        fh = gzopen(filename.c_str(), "wb");
    if (fh == Z_NULL)
    {
        std::cerr << "open gz file " << filename
                  << " failed, error: " << strerror(errno) << std::endl;
        return false;
    }

    for (auto& record : records)
    {
        // Add new line
        int writeBytes = gzputs(fh, record.c_str());
        if (writeBytes != record.size())
        {
            std::cerr << "write gz file " << filename
                      << " failed, data size: " << record.size()
                      << " write bytes: " << writeBytes << std::endl;
            return false;
        }
        gzputs(fh, detail::GZ_NEW_LINE);
    }

    gzclose(fh);
    return true;
}

}  // namespace libx