/*
 * File: FileTest.cpp
 * Created Data: 2021-7-15
 * Author: tigeroses
 */

#include "libx/File.hpp"
using libx::readFile;
#include "libx/System.hpp"
#include "libx/String.hpp"

#include <string>
#include <vector>
using namespace std;

#include <doctest.h>

TEST_SUITE("testing readFile and writeFile")
{
    TEST_CASE("readFile")
    {
        string filename("tmp_read.txt");
        // Create temp file for reading
        string createFileCmd = libx::join({"echo 'a b c\nd e f\n1 2 3\n' >", filename.c_str()}, " ");
        string output;
        int rtn = libx::subprocess(createFileCmd, output);
        REQUIRE(rtn == 0);
        
        // Read by line from temp file
        vector<string> records;
        auto readLine = [&](string& line) {
            records.push_back(line);
        };
        readFile("tmp_read.txt", readLine);
        CHECK(records.size() == 3);

        // Remove temp file
        string removeFileCmd = libx::join({"rm", filename.c_str()}, " ");
        output.clear();
        rtn = libx::subprocess(removeFileCmd, output);
        REQUIRE(rtn == 0);
    }
}