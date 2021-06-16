
/*
 * File: ThreadPool.hpp
 * Created Data: 2021-6-16
 * Author: tigeroses
 */

// Inspired by https://github.com/lzpong/threadpool.git

/*
 * ThreadPool is a thread pool for executing parallel tasks.
 *
 * You can init the pool with a specific size, then commit some tasks, finally 
 * get the results of tasks, for example,
 * 
 * ThreadPool thpool(20);
 *
 * vector<future<int>> results;
 * for (int i = 0; i < 40; ++i)
 *     results.emplace_back(thpool.commit(func, params...));
 *
 * for (auto& result : results)
 *     int num = result.get();
 *
 * For class static member function, use 
 * .commit(std::bind(&Dog::sayHello, &dog));
 * For class ordinary member function, use
 * .commit(Dog::sayHello);
 */

#pragma once

#include <atomic>
#include <future>
#include <queue>
#include <vector>
#include <condition_variable>
#include <thread>
#include <functional>
#include <stdexcept>
#include <cassert>

namespace libx {

class ThreadPool
{
private:
    using Task = std::function< void() >;    
    std::vector< std::thread >   _pool;      
    std::queue< Task >      _tasks;          
    std::mutex              _lock;           
    std::condition_variable _task_cv;        
    std::atomic< bool >     _run{ true };    
    std::atomic< int >      _idlThrNum{ 0 }; 

public:
    inline ThreadPool(unsigned short size)
    {
        assert(size > 0);
        addThread(size);
    }
    inline ~ThreadPool()
    {
        _run = false;
        _task_cv.notify_all();  // wake up all threads
        for (auto& thread : _pool)
        {
            if (thread.joinable())
                thread.join();  // wait all tasks finish
        }
    }

public:
    // Commit a task with a function and arguments
    template < class F, class... Args > 
    auto commit(F&& f, Args&&... args) -> std::future< decltype(f(args...)) >
    {
        if (!_run)
            throw std::runtime_error("commit on ThreadPool is stopped.");

        using RetType = decltype(f(args...));
        auto task     = std::make_shared< std::packaged_task< RetType() > >(
            std::bind(std::forward< F >(f), std::forward< Args >(args)...));
        std::future< RetType > future = task->get_future();
        {  
            // Add a task to the queue
            std::lock_guard< std::mutex > lock{ _lock };
            _tasks.emplace([task]() {
                (*task)();
            });
        }

        _task_cv.notify_one();  // wake up a thread to execute the task

        return future;
    }

    // The number of idle threads
    int idlCount()
    {
        return _idlThrNum;
    }

    // The number of total threads
    int thrCount()
    {
        return _pool.size();
    }

private:
    // Add threads with a specific size to the pool
    void addThread(unsigned short size)
    {
        while (size-- > 0)
        {
            _pool.emplace_back([this] {
                while (_run)
                {
                    Task task;  // get a task to be executed
                    {
                        std::unique_lock< std::mutex > lock{ _lock };
                        _task_cv.wait(lock, [this] { return !_run || !_tasks.empty(); });  // wait 直到有 task
                        if (!_run && _tasks.empty())
                            return;
                        task = std::move(_tasks.front());
                        _tasks.pop();
                    }
                    _idlThrNum--;
                    task();  // execute the task
                    _idlThrNum++;
                }
            });
            _idlThrNum++;
        }
    }
};

}  // namespace libx
