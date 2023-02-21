
/*
 * File: ThreadPoolTest.cpp
 * Created Data: 2021-6-16
 * Author: tigeroses
 */

#include "libx/ThreadPool.hpp"
#include "libx/Timer.hpp"

#include <thread>
#include <vector>

#include <doctest.h>

TEST_CASE("testing class of ThreadPool")
{
    SUBCASE("simple threads")
    {
        libx::Timer      timer("ms");
        libx::ThreadPool thpool(2);

        auto worker = [](int num)
        {
            auto sleepDuration = std::chrono::milliseconds{ num };
            std::this_thread::sleep_for(sleepDuration);

            return num;
        };

        std::vector< std::future< int > > results;
        results.emplace_back(thpool.commit(worker, 1000));
        results.emplace_back(thpool.commit(worker, 1500));
        results.emplace_back(thpool.commit(worker, 2000));
        results.emplace_back(thpool.commit(worker, 2500));

        int total = 0;
        for (auto&& result : results)
        {
            total += result.get();
        }

        CHECK(total == (1000 + 1500 + 2000 + 2500));
        // CHECK(timer.toc() == (1500 + 2500));
    }

    SUBCASE("class member function")
    {
        class A
        {
        public:
            static int f1(int num)
            {
                auto sleepDuration = std::chrono::milliseconds{ num };
                std::this_thread::sleep_for(sleepDuration);
                return num;
            }

            int f2(int num)
            {
                auto sleepDuration = std::chrono::milliseconds{ num };
                std::this_thread::sleep_for(sleepDuration);
                return num;
            }
        };

        libx::Timer      timer("ms");
        libx::ThreadPool thpool(2);
        CHECK(thpool.idlCount() == 2);
        CHECK(thpool.thrCount() == 2);

        std::vector< std::future< int > > results;
        A                                 a;
        results.emplace_back(thpool.commit(A::f1, 1000));
        results.emplace_back(thpool.commit(A::f1, 1500));
        results.emplace_back(thpool.commit(std::bind(&A::f2, a, 2000)));
        results.emplace_back(thpool.commit(std::bind(&A::f2, a, 2500)));

        int total = 0;
        for (auto&& result : results)
        {
            total += result.get();
        }

        CHECK(thpool.thrCount() == 2);
        CHECK(thpool.idlCount() == 2);

        CHECK(total == (1000 + 1500 + 2000 + 2500));
        // CHECK(timer.toc() == (1500 + 2500));
    }
}
