#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include "Observer.hpp"
#include "ThreadPool.hpp"

class ThreadPool;

class Worker
{
public:
	Worker(ThreadPool &s) : pool(s) {}
	void operator()();

private:
	ThreadPool &pool;
};

