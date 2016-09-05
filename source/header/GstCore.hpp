#pragma once
#include "GstManager.hpp"
#include <iostream>

class GstCore
{
public:
	GstCore(std::string urlH, std::string urlL, std::string uuid,
			std::string path);
	void run();

private:
	std::string getAction();

	/*! \brief Referência para o objeto responsável pelo gerenciamento dos
	 * frames.
	 *
	 */
	GstManager *gstManager;
};

