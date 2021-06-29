
/*
 * File: ConvTest.cpp
 * Created Data: 2021-6-4
 * Author: tigeroses
 */

#include <libx/Conv.hpp>

#include <doctest.h>

TEST_CASE("test function to()")
{
    // Convert string to numerical value
    std::string str("123");
    CHECK(libx::to< int >(str) == 123);
    CHECK(libx::to< float >(str) == 123.0);
    CHECK(libx::to< double >(str) == 123.0);
    CHECK(libx::to< long >(str) == 123);
    CHECK(libx::to< unsigned long >(str) == 123);
    CHECK(libx::to< long long >(str) == 123);
    CHECK(libx::to< unsigned long long >(str) == 123);
    CHECK(libx::to< std::string >(str) == "123");
}