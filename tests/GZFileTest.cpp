/*
 * File: GZFileTest.cpp
 * Created Data: 2021-7-20
 * Author: tigeroses
 */

#include <doctest.h>

#include <libx/GZFile.hpp>
using libx::readGZFile;
#include <libx/System.hpp>
#include <libx/String.hpp>

#include <string>
#include <set>
using namespace std;

TEST_SUITE("testing readGZFile")
{
    TEST_CASE("read gz file")
    {
        // Create a temp gz file
        string textFile("temp_gz_file.txt");
        string createFileCmd("echo 'a b c\nd\ne f\n' > " + textFile);
        REQUIRE(libx::subprocess(createFileCmd) == 0);
        string gzipFile(textFile + ".gz");
        string compressCmd("gzip -fq " + textFile);
        REQUIRE(libx::subprocess(compressCmd) == 0);

        // Testing
        SUBCASE("just read one line")
        {
            string oneline;
            auto readOneLine = [&](string& line){
                oneline = line;
                return false;
            };
            readGZFile(gzipFile, readOneLine);
            CHECK(oneline == "a b c");
        }

        SUBCASE("read line by line")
        {
            vector<string> lines;
            auto readLines = [&](string& line){
                lines.push_back(line);
                return true;
            };
            readGZFile(gzipFile, readLines);
            CHECK(lines.size() == 3);
            CHECK(lines[0] == "a b c");
            CHECK(lines[1] == "d");
            CHECK(lines[2] == "e f");
        }

        SUBCASE("split each line")
        {
            set<string> uniqValues;
            auto readUniqValues = [&](string& line){
                vector<string> res;
                libx::split(line, ' ', res);
                for (auto& v : res)
                    uniqValues.insert(v);
                return true;
            };
            readGZFile(gzipFile, readUniqValues);
            CHECK(uniqValues.size() == 6);
        }

        // Remove temp file
        string removeCmd("rm " + gzipFile);
        REQUIRE(libx::subprocess(removeCmd) == 0);
    }
}