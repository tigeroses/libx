
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

TEST_CASE("testing function split")
{
    SUBCASE("char delim")
    {
        string in = "a:bc:def::g:";
        vector<string> res;
        split(in, ':', res);
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
        split(in, ":", res);
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
        split(in, "  ", res);
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
        split(in, ' ', res);
        CHECK(res.size() == 0);
    }

    SUBCASE("single input")
    {
        string in = "a";
        vector<string> res;
        split(in, 'a', res);
        CHECK(res.size() == 0);
        split(in, 'a', res, false);
        CHECK(res.size() == 2);
        CHECK(res[0].empty());
        CHECK(res[1].empty());
    }

    SUBCASE("integer as ouput")
    {
        string in = "1:2:34:567";
        vector<int> res;
        split(in, ':', res);
        CHECK(res.size() == 4);
        CHECK(res[0] == 1);
        CHECK(res[1] == 2);
        CHECK(res[2] == 34);
        CHECK(res[3] == 567);
    }
}

TEST_CASE("testing function split fixed")
{
    SUBCASE("integer as ouput")
    {
        string in = "1:2:34:567";
        int a,b,c,d;
        CHECK(split(in, ':', a, b, c, d));
        CHECK(a == 1);
        CHECK(b == 2);
        CHECK(c == 34);
        CHECK(d == 567);
    }

    SUBCASE("conversion exception")
    {
        string in = "23333333333";
        int a;
        CHECK_THROWS_AS(split(in, ':', a), const std::exception&);
    }
    
    SUBCASE("multiple output types")
    {
        string in = "1:2.0:23333333333:abc";
        int a;
        float b;
        long long c;
        string d;
        
        CHECK(split(in, ':', a, b, c, d));
        CHECK(a == 1);
        CHECK(b == 2.0);
        CHECK(c == 23333333333);
        CHECK(d == "abc");
    }
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