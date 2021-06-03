
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
    string in = "a:bc:def::g:";
    vector<string> res;
    split(in, res, ":");
    CHECK(res.size() == 4);
    CHECK(res[0] == "a");
    CHECK(res[1] == "bc");
    CHECK(res[2] == "def");
    CHECK(res[3] == "g");

    res.clear();
    split(in, res, ':');
    CHECK(res.size() == 4);
    CHECK(res[0] == "a");
    CHECK(res[1] == "bc");
    CHECK(res[2] == "def");
    CHECK(res[3] == "g");

    string s;
    std::cout<<typeid(s).name()<<std::endl;
    char c;
    std::cout<<typeid(c).name()<<std::endl;
    std::cout<<typeid(":").name()<<std::endl;
    std::cout<<typeid(':').name()<<std::endl;
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