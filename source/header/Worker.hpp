#ifndef WORKER_H
#define WORKER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include "ThreadPool.hpp"
#include "Observer.hpp"

class ThreadPool;

class Worker {
public:
    Worker(ThreadPool &s) : pool(s) { }
    void operator()();
private:
    ThreadPool &pool;
};

#endif