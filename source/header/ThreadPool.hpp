#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <deque>
#include "Worker.hpp"
#include "Observer.hpp"

class ThreadPool {
public:
    // Singleton pattern
    static ThreadPool* getThreadPool();
    void operator=(ThreadPool const&) =  delete; // Desabling copy operator

   void enqueue(Observer* observer);
    static bool mustStop() { return _stop; }
    ~ThreadPool();

private:
    friend class Worker;

    // Singleton pattern
    static ThreadPool* _instance;
    ThreadPool(size_t);
    ThreadPool();
    static std::mutex _instaceMutex;
 
    // need to keep track of threads so we can join them
    std::vector<std::thread> _workers;
 
    // the task queue
    // std::deque<std::function<void()>> tasks;
    std::deque<Observer*> _observers;
 
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    static bool _stop;
};

