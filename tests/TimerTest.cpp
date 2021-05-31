
/*
 * File: TestTimer.h
 * Created Data: 2021-5-28
 * Author: tigeroses
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "libx/Timer.hpp"

#include <thread>

using libx::Timer;

TEST_CASE("testing the Timer")
{
    Timer timer1("mics");
    Timer timer2("ms");
    Timer timer3("s");
    Timer timer4;

    auto sleepDuration = std::chrono::milliseconds{ 1000 };
    std::this_thread::sleep_for(sleepDuration);

    CHECK(timer1.toc() >= 1000000);
    CHECK(timer2.toc() >= 1000);
    CHECK(timer3.toc() >= 1);
    CHECK(timer4.toc() >= 1);
}