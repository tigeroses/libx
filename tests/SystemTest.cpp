/*
 * File: SystemTest.cpp
 * Created Data: 2021-7-13
 * Author: tigeroses
 */

#include "libx/System.hpp"
using libx::getProcessMemory;
using libx::getSelfMemory;
using libx::getSystemMemory;
using libx::subprocess;

#include <fstream>
#include <string>
#include <thread>
using namespace std;

#include <doctest.h>

TEST_SUITE("testing system subprocess")
{
    TEST_CASE("call shell")
    {
        SUBCASE("call command")
        {
            string cmd("ls -l");
            string out;
            int    rtn = subprocess(cmd, out);
            CHECK(rtn == 0);
            CHECK(!out.empty());
        }
        SUBCASE("call shell script")
        {
            string cmd("for i in {1..10}; do echo 'hello';done");
            string out;
            int    rtn = subprocess(cmd, out);
            CHECK(rtn == 0);
            string tmp;
            for (int i = 0; i < 10; ++i)
                tmp += "hello\n";
            CHECK(out == tmp);
        }
    }

    TEST_CASE("call python")
    {
        string cmd("python -c \"print 'hello world!'\"");
        string out;
        int    rtn = subprocess(cmd, out);
        CHECK(rtn == 0);
        CHECK(!out.empty());
        CHECK(out == "hello world!\n");
    }

    TEST_CASE("call non-exist application")
    {
        string cmd("abcdefg");
        string out;
        SUBCASE("just get stdout")
        {
            cmd.append(" 2>/dev/null");
            int rtn = subprocess(cmd, out);
            CHECK(rtn != 0);
            CHECK(out.empty());
        }
        SUBCASE("get stdout and stderr")
        {
            cmd.append(" 2>&1");
            int rtn = subprocess(cmd, out);
            CHECK(rtn != 0);
            CHECK(!out.empty());
            CHECK(out == "sh: abcdefg: command not found\n");
        }
    }
}

TEST_SUITE("testing system compareFileTime")
{
    TEST_CASE("normal test")
    {
        fs::path p("/dev/shm");
        fs::path f1 = p / "file1";
        fs::path f2 = p / "file2";

        SUBCASE("same time")
        {
            ofstream(f1.c_str()).put('a');
            ofstream(f2.c_str()).put('b');

            CHECK(libx::compareFileTime(f1, f2));
            CHECK(libx::compareFileTime(f2, f1));
        }
        SUBCASE("sleep between file creation")
        {
            ofstream(f1.c_str()).put('a');

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            ofstream(f2.c_str()).put('b');

            CHECK(libx::compareFileTime(f1, f2));
            CHECK(!libx::compareFileTime(f2, f1));
        }

        fs::remove(f1);
        fs::remove(f2);
    }
}

TEST_SUITE("testing system genRandomString")
{
    TEST_CASE("genRandomString")
    {
        SUBCASE("normal")
        {
            int    len = 10;
            string s1  = libx::genRandomString(len);
            string s2  = libx::genRandomString(len);

            REQUIRE(s1.size() == len);
            REQUIRE(s2.size() == len);
            REQUIRE(s1 != s2);
            for (auto& s : { s1, s2 })
            {
                for (auto& c : s)
                {
                    bool validChar = (48 <= c && c <= 57)
                                     || (65 <= c && c <= 90)
                                     || (97 <= c && c <= 122);
                    CHECK(validChar);
                }
            }
        }
    }
}

TEST_SUITE("testing system memory")
{
    TEST_CASE("example")
    {
        CHECK(getSystemMemory() > 0);
    }
}

TEST_SUITE("testing process memory")
{
    TEST_CASE("self memory")
    {
        CHECK(getSelfMemory() == 0);
    }
    TEST_CASE("other process memory")
    {
        double      vm, pm;
        std::string unit("BYTE");
        int         pid = 1;  // init process, it always exists
        getProcessMemory(vm, pm, unit, pid);
        CHECK(pm > 0);
        CHECK(vm > 0);
    }
}