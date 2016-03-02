#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <memory>
// #include "FramesManager.hpp"
// #include <list>
#include <pthread.h>
#include <vector>
#include <thread>

static void do_something();

int main(int argc, char *argv[])
{
	std::vector<std::thread> myVector;

	for(int i = 0; i < 1; i++)
	{
		myVector.push_back(std::thread(do_something));
	}


	for(int i = 0; i < 1; i++)
	{
		myVector[i].join();
	}

	// std::list<int*> observer;
	// int* i = new int(900);

	// observer.push_back(i);
	// observer.push_back(new int(1));
	// observer.push_back(new int(2));
	// observer.push_back(new int(3));
	// observer.push_back(new int(4));
	// observer.push_back(new int(5));
	// observer.push_back(new int(6));
	// observer.push_back(new int(7));

	// std::list<int*>::iterator it;

	// for(it = observer.begin(); it != observer.end(); it++)
	// {
	// 	std::cout << **it << std::endl;
	// }

	// *i = 40;

	// for(it = observer.begin(); it != observer.end(); it++)
	// {
	// 	std::cout << **it << std::endl;
	// }

	// observer.remove(i);

	// for(it = observer.begin(); it != observer.end(); it++)
	// {
	// 	std::cout << **it << std::endl;
	// }
}

void do_something()
{
}