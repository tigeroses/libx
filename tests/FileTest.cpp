/*
 * File: FileTest.cpp
 * Created Data: 2021-7-15
 * Author: tigeroses
 */

#include "libx/File.hpp"
using libx::readFile;
using libx::writeFile;
#include "libx/String.hpp"
#include "libx/System.hpp"

#include <string>
#include <vector>
using namespace std;

#include <doctest.h>

TEST_SUITE("testing readFile")
{
    TEST_CASE("file not exsits")
    {
        string           filename("file_not_exists.txt");
        vector< string > records;
        auto             readLine = [&](string& line)
        {
            records.push_back(line);
            return true;
        };
        CHECK(!readFile(filename, readLine));
        CHECK(records.size() == 0);
    }
    TEST_CASE("readFile")
    {
        string filename("tmp_file_simple.txt");
        // Create temp file for reading
        string createFileCmd = libx::join(
            { "echo 'a b c\nd e f\n1 2 3\n' >", filename.c_str() }, " ");
        int rtn = libx::subprocess(createFileCmd);
        REQUIRE(rtn == 0);

        SUBCASE("read file by lines")
        {
            // Read by line from temp file
            vector< string > records;
            auto             readLine = [&](string& line)
            {
                records.push_back(line);
                return true;
            };
            CHECK(readFile(filename, readLine));
            CHECK(records.size() == 3);
        }

        SUBCASE("read file by tokens")
        {
            // Read by tokens from temp file
            vector< string > records;
            auto             readLine = [&](string& line)
            {
                vector< string > res;
                libx::split(line, ' ', res);
                for (auto& s : res)
                    records.push_back(s);
                return true;
            };
            readFile(filename, readLine);
            CHECK(records.size() == 9);
        }

        // Remove temp file
        string removeFileCmd = libx::join({ "rm", filename.c_str() }, " ");
        rtn                  = libx::subprocess(removeFileCmd);
        REQUIRE(rtn == 0);
    }

    TEST_CASE("read file with comment")
    {
        string filename("tmp_file_comment.txt");
        // Create temp file for reading
        string createFileCmd =
            libx::join({ "echo '#this is comment\na b c\nd e f\n1 2 3\n' >",
                         filename.c_str() },
                       " ");
        int rtn = libx::subprocess(createFileCmd);
        REQUIRE(rtn == 0);

        vector< string > records;
        auto             readLine = [&](string& line)
        {
            records.push_back(line);
            return true;
        };

        SUBCASE("default skip comment")
        {
            readFile(filename, readLine);
            CHECK(records.size() == 3);
        }
        SUBCASE("specify comment type")
        {
            readFile(filename, readLine, "#");
            CHECK(records.size() == 3);
        }
        SUBCASE("specify comment type")
        {
            readFile(filename, readLine, "#t");
            CHECK(records.size() == 3);
        }
        SUBCASE("specify comment type")
        {
            readFile(filename, readLine, "#T");
            CHECK(records.size() == 4);
        }
        SUBCASE("use skipLineNum")
        {
            readFile(filename, readLine, "-", 1);
            CHECK(records.size() == 3);
        }
        SUBCASE("use skipLineNum and comment")
        {
            readFile(filename, readLine, "#", 1);
            CHECK(records.size() == 2);
        }
        // Remove temp file
        string removeFileCmd = libx::join({ "rm", filename.c_str() }, " ");
        rtn                  = libx::subprocess(removeFileCmd);
        REQUIRE(rtn == 0);
    }
}

TEST_SUITE("testing writeFile")
{
    TEST_CASE("testing writeFile")
    {
        string           filename("write_file.txt");
        vector< string > records;
        auto             readLine = [&](string& line)
        {
            records.push_back(line);
            return true;
        };

        SUBCASE("file not exists")
        {
            REQUIRE(writeFile("a b c\nd e f\n", filename));

            CHECK(readFile(filename, readLine));
            CHECK(records.size() == 2);
        }
        SUBCASE("write from start of file")
        {
            string createFileCmd = libx::join(
                { "echo 'a b c\nd e f\n1 2 3\n' >", filename.c_str() }, " ");
            REQUIRE(libx::subprocess(createFileCmd) == 0);

            REQUIRE(writeFile("a b c\nd e f\n", filename));

            CHECK(readFile(filename, readLine));
            CHECK(records.size() == 2);
        }
        SUBCASE("append to file")
        {
            string createFileCmd = libx::join(
                { "echo 'a b c\nd e f\n1 2 3\n' >", filename.c_str() }, " ");
            REQUIRE(libx::subprocess(createFileCmd) == 0);

            REQUIRE(writeFile("a b c\nd e f\n", filename, true));

            CHECK(readFile(filename, readLine));
            CHECK(records.size() == 5);
        }
        SUBCASE("append to file using vector<string>")
        {
            string createFileCmd = libx::join(
                { "echo 'a b c\nd e f\n1 2 3\n' >", filename.c_str() }, " ");
            REQUIRE(libx::subprocess(createFileCmd) == 0);

            vector< string > inputsString{ "a b c", "d e f" };
            REQUIRE(writeFile(inputsString, filename, true));

            vector< int > inputsInt{ 1, 200 };
            REQUIRE(writeFile(inputsInt, filename, true));

            CHECK(readFile(filename, readLine));
            CHECK(records.size() == 7);
        }

        // Remove temp file
        string removeFileCmd = libx::join({ "rm", filename.c_str() }, " ");
        REQUIRE(libx::subprocess(removeFileCmd) == 0);
    }
}