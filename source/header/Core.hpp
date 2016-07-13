#pragma once

#include "FramesManager.hpp"
#include "SocketCommunicator.hpp"
#include <iostream>
#include <vector>

class Core
{
public:
	Core(std::string url, std::string model, std::string portNumber);
	void run();

private:
	std::string getAction();

	/*! \brief Referência para o objeto responsável pelo gerenciamento dos
	 * frames.
	 *
	 */
	FramesManager *_framesManager;

	/*! \brief Referência para o objeto responsável pela comunicação via socket
	 * com o Mid.
	 *
	 */
	SocketCommunicator *_socketCommunicator;
};

