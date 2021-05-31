
/*
 * File: Timer.h
 * Created Data: 2021-5-28
 * Author: tigeroses
 */

#pragma once

#include <mutex>
#include <thread>
#include <string>

namespace libx {

class Timer
{
public:
    Timer(std::string unit="s") : unit(unit), start_time(mics_duration::zero())
    { 
        div = 1;
        if (unit == "s")
            div = 1e6;
        else if (unit == "ms")
            div = 1e3;
        else if (unit == "mics")
            div = 1;
        
        tic();
    }

    ~Timer() {}

    // Start a new timer
    void tic()
    {
        start_time = clock::now();
    }

    // Calculate the duration after last tic(), and start a new timer
    double toc()
    {
        double duration = 
            std::chrono::duration_cast< mics_duration >(clock::now() - start_time).count() / div;
        
        tic();

        return duration;
    }

private:
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::microseconds          mics_duration;

    clock::time_point start_time;

    std::string unit;
    unsigned int div;
};

} // namespace libx