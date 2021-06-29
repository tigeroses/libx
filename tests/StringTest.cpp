
/*
 * File: StringTest.cpp
 * Created Data: 2021-6-1
 * Author: tigeroses
 */

#include "libx/String.hpp"
#include "libx/Conv.hpp"
#include "libx/Timer.hpp"
using libx::split;
using libx::Timer;

#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;

#include <doctest.h>

struct ST
{
    ST(int i_, std::string s_) : i(i_), s(s_) {}
    int         i;
    std::string s;
};
// Implement libx::toStr(SrcType) for using libx::join
namespace libx
{
template <>
inline std::string toStr(ST st)
{
    return toStr(st.i) + "_" + st.s;
}
}  // namespace libx

TEST_SUITE_BEGIN("testing split strings");

TEST_CASE("testing function split")
{
    SUBCASE("char delim")
    {
        string           in = "a:bc:def::g:";
        vector< string > res;
        split(in, ':', res);
        CHECK(res.size() == 4);
        CHECK(res[0] == "a");
        CHECK(res[1] == "bc");
        CHECK(res[2] == "def");
        CHECK(res[3] == "g");
    }

    SUBCASE("char as string delim")
    {
        string           in = "a:bc:def::g:";
        vector< string > res;
        split(in, ":", res);
        CHECK(res.size() == 4);
        CHECK(res[0] == "a");
        CHECK(res[1] == "bc");
        CHECK(res[2] == "def");
        CHECK(res[3] == "g");
    }

    SUBCASE("string delim")
    {
        string           in = "   a b  cd      efg    h ";
        vector< string > res{ "1", "2", "3", "4" };
        split(in, "  ", res);
        CHECK(res.size() == 8);
        CHECK(res[4] == " a b");
        CHECK(res[5] == "cd");
        CHECK(res[6] == "efg");
        CHECK(res[7] == "h ");
    }

    SUBCASE("empty input")
    {
        string           in = "";
        vector< string > res;
        split(in, ' ', res);
        CHECK(res.size() == 0);
    }

    SUBCASE("single input")
    {
        string           in = "a";
        vector< string > res;
        split(in, 'a', res);
        CHECK(res.size() == 0);
        split(in, 'a', res, false);
        CHECK(res.size() == 2);
        CHECK(res[0].empty());
        CHECK(res[1].empty());
    }

    SUBCASE("integer as ouput")
    {
        string        in = "1:2:34:567";
        vector< int > res;
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
        int    a, b, c, d;
        CHECK(split(in, ':', a, b, c, d));
        CHECK(a == 1);
        CHECK(b == 2);
        CHECK(c == 34);
        CHECK(d == 567);
    }

    SUBCASE("conversion exception")
    {
        string in = "23333333333";
        int    a;
        CHECK_THROWS_AS(split(in, ':', a), const std::exception&);
    }

    SUBCASE("multiple output types")
    {
        string    in = "1:2.0:23333333333:abc";
        int       a;
        float     b;
        long long c;
        string    d;

        CHECK(split(in, ':', a, b, c, d));
        CHECK(a == 1);
        CHECK(b == 2.0);
        CHECK(c == 23333333333);
        CHECK(d == "abc");
    }
}

TEST_CASE("testing performance of split()" * doctest::skip(true))
{
    Timer            timer("ms");
    string           in    = "NOC2L\t93277\t37070\t1";
    int              times = 5000000;
    vector< string > res;
    while (times-- > 0)
    {
        res.clear();
        split(in, '\t', res);
    }
    std::cout << "time split: " << timer.toc() << std::endl;
}

TEST_SUITE_END();

TEST_SUITE("testing join into string")
{
    TEST_CASE("same type as inputs")
    {
        std::string out;
        SUBCASE("string type and char delimiter")
        {
            std::vector< std::string > inputs{ "a", "bc", "def" };
            libx::join(inputs, ' ', out);
            CHECK(out == "a bc def");
        }
        SUBCASE("string type and string delimiter")
        {
            libx::join({ "a", "bc", "def" }, "__", out);
            CHECK(out == "a__bc__def");
        }
        SUBCASE("int type")
        {
            libx::join({ 1, 23, 456 }, '\t', out);
            CHECK(out == "1\t23\t456");
        }
        SUBCASE("float type")
        {
            libx::join({ 1.01, 23.888, 456.789987 }, ',', out);
            CHECK(out == "1.010000,23.888000,456.789987");
        }

        SUBCASE("user-defined type")
        {
            ST st1(1, "abc"), st2(2, "def");
            CHECK(libx::join({ st1, st2 }, " ") == "1_abc 2_def");
        }
        SUBCASE("empty input")
        {
            std::vector< int > inputs;
            CHECK(libx::join(inputs, ' ').empty());
        }
        SUBCASE("single input")
        {
            CHECK(libx::join({ "abc" }, ' ') == "abc");
        }
    }

    TEST_CASE("different type as inputs")
    {
        SUBCASE("simple test")
        {
            int         i = 10;
            std::string s = "libx";
            CHECK(libx::join(' ', i, s) == "10 libx");
        }
        SUBCASE("simple test")
        {
            std::string out;
            int         i = 10;
            float       f = 3.33;
            std::string s = "libx";
            out           = libx::join(' ', i, f, s);
            CHECK(out == "10 3.330000 libx");
        }
    }
}