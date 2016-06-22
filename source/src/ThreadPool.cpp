#include "ThreadPool.hpp"
#include <iostream>

// const size_t threads = 2;

ThreadPool *ThreadPool::_instance = 0;
std::mutex ThreadPool::_instaceMutex;
bool ThreadPool::_stop = false;

ThreadPool *ThreadPool::getThreadPool()
{
	std::unique_lock<std::mutex> _lock(_instaceMutex);

	if (_instance == 0)
	{
		_instance = new ThreadPool();
		// _instance = new ThreadPool(threads);
	}

	return _instance;
}

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t numberOfThreads)
// :   _stop(false)
{
	for (size_t i = 0; i < numberOfThreads; ++i)
		_workers.push_back(std::thread(Worker(*this)));
}

// the constructor just launches some amount of workers
ThreadPool::ThreadPool()
// :   _stop(false)
{
	size_t concurentThreadsSupported =
		(size_t)std::thread::hardware_concurrency();
	size_t numberOfThreads = 2;

	if (concurentThreadsSupported > 0)
		numberOfThreads = concurentThreadsSupported;

	for (size_t i = 0; i < numberOfThreads; ++i)
		_workers.push_back(std::thread(Worker(*this)));
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
	// _stop all threads
	_stop = true;
	condition.notify_all();

	// join them
	for (size_t i = 0; i < _workers.size(); ++i)
		_workers[i].join();
}

// add new work item to the pool

// template<class R, class... Args >
// void ThreadPool::enqueue(std::function<R(Args...)> f)
// void ThreadPool::enqueue(std::function<void()> f)
// void ThreadPool::enqueue(Observer* observer, FramesManager* framesManager)
void ThreadPool::enqueue(Observer *observer)
{
	{  // acquire lock
		std::unique_lock<std::mutex> lock(queue_mutex);

		// add the task
		_observers.push_back(observer);
	}  // release lock

	// wake up one thread
	condition.notify_one();
}

