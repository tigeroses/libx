
/*
 * File: StringTest.cpp
 * Created Data: 2021-6-1
 * Author: tigeroses
 */

#include "libx/String.hpp"
#include "libx/Timer.hpp"
using libx::split;
using libx::Timer;

#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;

#include <doctest.h>

TEST_CASE("testing function split()")
{
    SUBCASE("char delim")
    {
        string in = "a:bc:def::g:";
        vector<string> res;
        split(in, res, ':');
        CHECK(res.size() == 4);
        CHECK(res[0] == "a");
        CHECK(res[1] == "bc");
        CHECK(res[2] == "def");
        CHECK(res[3] == "g");
    }

    SUBCASE("char as string delim")
    {
        string in = "a:bc:def::g:";
        vector<string> res;
        split(in, res, ":");
        CHECK(res.size() == 4);
        CHECK(res[0] == "a");
        CHECK(res[1] == "bc");
        CHECK(res[2] == "def");
        CHECK(res[3] == "g");
    }

    SUBCASE("string delim")
    {
        string in = "   a b  cd      efg    h ";
        vector<string> res{"1", "2", "3", "4"};
        split(in, res, "  ");
        CHECK(res.size() == 8);
        CHECK(res[4] == " a b");
        CHECK(res[5] == "cd");
        CHECK(res[6] == "efg");
        CHECK(res[7] == "h ");
    }

    SUBCASE("empty input")
    {
        string in = "";
        vector<string> res;
        split(in, res, ' ');
        CHECK(res.size() == 0);
    }

    SUBCASE("single input")
    {
        string in = "a";
        vector<string> res;
        split(in, res, 'a');
        CHECK(res.size() == 0);
        split(in, res, 'a', false);
        CHECK(res.size() == 2);
        CHECK(res[0].empty());
        CHECK(res[1].empty());
    }

    // SUBCASE("integer as ouput")
    // {
    //     string in = "1:2:34:567";
    //     vector<int> res;
    //     split(in, res, ':');
    //     CHECK(res.size() == 4);
    // }

    // SUBCASE("float as ouput")
    // {
        
    // }
}

// TEST_CASE("testing performance of split()")
// {
//     Timer timer("ms");
//     string in = "NOC2L\t93277\t37070\t1";
//     int times = 5000000;
//     vector<string> res;
//     while (times-- > 0)
//     {
//         res.clear();
//         split(in, res, '\t');
//     }
//     std::cout<<"time1: "<<timer.toc()<<std::endl;
// }