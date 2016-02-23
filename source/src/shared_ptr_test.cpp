#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <memory>
// #include "FramesManager.hpp"
#include <list>

int main(int argc, char *argv[])
{
	std::list<int*> observer;
	int* i = new int(900);

	observer.push_back(i);
	observer.push_back(new int(1));
	observer.push_back(new int(2));
	observer.push_back(new int(3));
	observer.push_back(new int(4));
	observer.push_back(new int(5));
	observer.push_back(new int(6));
	observer.push_back(new int(7));

	std::list<int*>::iterator it;

	for(it = observer.begin(); it != observer.end(); it++)
	{
		std::cout << **it << std::endl;
	}

	*i = 40;

	for(it = observer.begin(); it != observer.end(); it++)
	{
		std::cout << **it << std::endl;
	}

	observer.remove(i);


	// for(it = observer.begin(); it != observer.end(); it++)
	// {
	// 	if(it == *i)
	// 	{
	// 		observer.remove(i);
	// 	}
	// }

	for(it = observer.begin(); it != observer.end(); it++)
	{
		std::cout << **it << std::endl;
	}
}