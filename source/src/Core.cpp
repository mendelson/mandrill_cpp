#include <opencv2/opencv.hpp>
#include <thread>
#include "Core.hpp"
#include "Analyser.hpp"
#include "Printer.hpp"

static void framesManagerRunHelper(FramesManager* framesManager);

Core::Core(std::string url, std::string model)
{
	// std::cout << "framesManager lixo: " << _framesManager << std::endl;
	// std::cout << "retorno: " << FramesManager::getManager() << std::endl;
	_framesManager = FramesManager::getManager();
	_framesManager->setStreamSource(url, model);
	// std::cout << "nha1 " << _framesManager->nha << std::endl;
	// std::cout << "framesManager no construtor: " << _framesManager << std::endl;
}

void Core::run()
{
	// std::cout << "framesManager na run: " << _framesManager << std::endl;

	Analyser* anal = new Analyser();
	Printer* printer = new Printer();
	_framesManager->Attach(anal);
	// FramesManager::getManager()->Attach(anal);
	_framesManager->Attach(printer);
	
	_threadVector->emplace_back(std::thread(framesManagerRunHelper, _framesManager));

	// for(int i = 0; i < 100; i++)
	// {
	// 	std::cout << "hey" << std::endl;
	// }

	// if(_framesManager == FramesManager::getManager())
	// {
		// std::cout << "Sao iguais!" << std::endl;
	// }
	// else if (_framesManager != FramesManager::getManager())
	// {
		// std::cout << "Sao diferentes!" << std::endl;
	// }

	// std::cout << "nha2 " << _framesManager->nha << std::endl;
	// std::cout << "nha02 " << FramesManager::getManager()->nha << std::endl;

	// std::thread(framesManagerRunHelper, _framesManager).detach();
	// framesManagerRunHelper(_framesManager);
	// std::cout << "nha3 " << _framesManager->nha << std::endl;
	// (*_threadVector)[0].join();
	// std::cout << "ha" << std::endl;


	// std::string test = getAction();

	// std::cout << "ha" << std::endl;
	// int i = 0;
	// while(true)
	// {
	// 	std::cout << i << std::endl;
	// 	i++;
	// }

	// joinAllThreads();
	// std::cout << "ha" << std::endl;
	// (*_threadVector)[0].join();
	while(true);

}

void framesManagerRunHelper(FramesManager* framesManager)
{
	framesManager->run();
}

std::string Core::getAction()
{
	std::string action;

	getline(std::cin, action);

	std::cout << action << std::endl;

	return action;
}

void Core::killThread(std::thread target)
{
	// TODO
}

void Core::joinAllThreads()
{
	for(unsigned int i = 0; i < (*_threadVector).size(); i++)
	{
		(*_threadVector)[i].join();
	}
}