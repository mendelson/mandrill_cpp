#pragma once

#include "Observer.hpp"
#include "ThreadPool.hpp"
#include <condition_variable>
#include <mutex>
#include <thread>

class ThreadPool;

class Worker
{
public:
	Worker(ThreadPool &s) : pool(s) {}
	void operator()();

private:
	ThreadPool &pool;
};

