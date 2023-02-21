/*
 * File: GZFileTest.cpp
 * Created Data: 2021-7-20
 * Author: tigeroses
 */

#include <doctest.h>

#include <libx/GZFile.hpp>
using libx::readGZFile;
using libx::writeGZFile;
#include <libx/String.hpp>
#include <libx/System.hpp>

#include <set>
#include <string>
using namespace std;

TEST_SUITE("testing readGZFile")
{
    TEST_CASE("read gz file")
    {
        // Create a temp gz file
        string textFile("temp_gz_file.txt");
        string createFileCmd("echo '#this is comment\na b c\nd\ne f\n' > "
                             + textFile);
        REQUIRE(libx::subprocess(createFileCmd) == 0);
        string gzipFile(textFile + ".gz");
        string compressCmd("gzip -fq " + textFile);
        REQUIRE(libx::subprocess(compressCmd) == 0);

        // Testing
        SUBCASE("just read one line")
        {
            string oneline;
            auto   readOneLine = [&](string& line)
            {
                oneline = line;
                return false;
            };
            readGZFile(gzipFile, readOneLine);
            CHECK(oneline == "a b c");
        }

        SUBCASE("read line by line")
        {
            vector< string > lines;
            auto             readLines = [&](string& line)
            {
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
            set< string > uniqValues;
            auto          readUniqValues = [&](string& line)
            {
                vector< string > res;
                libx::split(line, ' ', res);
                for (auto& v : res)
                    uniqValues.insert(v);
                return true;
            };
            readGZFile(gzipFile, readUniqValues);
            CHECK(uniqValues.size() == 6);
        }

        SUBCASE("split each line with skipLineNum")
        {
            set< string > uniqValues;
            auto          readUniqValues = [&](string& line)
            {
                vector< string > res;
                libx::split(line, ' ', res);
                for (auto& v : res)
                    uniqValues.insert(v);
                return true;
            };
            readGZFile(gzipFile, readUniqValues, "#", 1);
            CHECK(uniqValues.size() == 3);
        }

        SUBCASE("split each line with skipLineNum")
        {
            set< string > uniqValues;
            auto          readUniqValues = [&](string& line)
            {
                vector< string > res;
                libx::split(line, ' ', res);
                for (auto& v : res)
                    uniqValues.insert(v);
                return true;
            };
            readGZFile(gzipFile, readUniqValues, "#", 2);
            CHECK(uniqValues.size() == 2);
        }

        // Remove temp file
        string removeCmd("rm " + gzipFile);
        REQUIRE(libx::subprocess(removeCmd) == 0);
    }
}

TEST_SUITE("tesing writeGZFile")
{
    TEST_CASE("write gz file")
    {
        string gzipFile("temp_gz_file.gz");
        REQUIRE(libx::subprocess("zcat -h 2>&1") == 0);
        string checkCmd("zcat " + gzipFile);
        string records;

        SUBCASE("write string")
        {
            REQUIRE(writeGZFile("abc\ndefghi\n123\n4567\n", gzipFile));

            // Check write is ok
            REQUIRE(libx::subprocess(checkCmd, records) == 0);
            CHECK(records == "abc\ndefghi\n123\n4567\n");
        }
        SUBCASE("write vector of string")
        {
            vector< string > datas{ "abc", "defghi", "123", "4567" };
            REQUIRE(writeGZFile(datas, gzipFile));

            // Check write is ok
            REQUIRE(libx::subprocess(checkCmd, records) == 0);
            CHECK(records == "abc\ndefghi\n123\n4567\n");
        }
        SUBCASE("append mode")
        {
            REQUIRE(writeGZFile("abc\ndefghi\n123\n4567\n", gzipFile));
            vector< string > datas{ "abc", "defghi", "123", "4567" };
            REQUIRE(writeGZFile(datas, gzipFile, true));

            // Check write is ok
            REQUIRE(libx::subprocess(checkCmd, records) == 0);
            CHECK(records
                  == "abc\ndefghi\n123\n4567\nabc\ndefghi\n123\n4567\n");
        }

        // Remove temp file
        string removeCmd("rm " + gzipFile);
        REQUIRE(libx::subprocess(removeCmd) == 0);
    }
}