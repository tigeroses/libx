/*
 * File: SystemTest.cpp
 * Created Data: 2021-7-13
 * Author: tigeroses
 */

#include "libx/System.hpp"
using libx::subprocess;

#include <string>
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
            int rtn = subprocess(cmd, out);
            CHECK(rtn == 0);
            CHECK(!out.empty());
        }
        SUBCASE("call shell script")
        {
            string cmd("for i in {1..10}; do echo 'hello';done");
            string out;
            int rtn = subprocess(cmd, out);
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
        int rtn = subprocess(cmd, out);
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