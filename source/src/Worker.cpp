#include "Worker.hpp"

// void Worker::operator()()
// {
//     std::function<void()> task;
//     while(true)
//     {
//         {   // acquire lock
//             std::unique_lock<std::mutex>
//             lock(pool.queue_mutex);

//             // look for a work item
//             while(!pool._stop &&
//             pool.tasks.empty())
//             { // if there are none
//             wait for notification
//                 pool.condition.wait(lock);
//             }

//             if(pool._stop) // exit if
//             the pool is stopped
//                 return;

//             // get the task from the
//             queue
//             task =
//             pool.tasks.front();
//             pool.tasks.pop_front();

//         }   // release lock

//         // execute the task
//         task();
//     }
// }

void Worker::operator()()
{
	Observer *task;
	while(true)
	{
		task = nullptr;

		{  // acquire lock
			std::unique_lock<std::mutex> lock(pool.queue_mutex);

			// look for a work item
			while(!pool._stop && pool._observers.empty())
			{  // if there are none wait
				// for notification
				pool.condition.wait(lock);
			}

			if(pool._stop)  // exit if
				// the pool
				// is
				// stopped
				return;

			// get the task from the
			// queue
			task = pool._observers.front();
			pool._observers.pop_front();

		}  // release lock

		// execute the task
		task->Update();
	}
}

