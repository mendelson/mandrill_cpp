#pragma once

#include <string>

class Printer
{
public:
	Printer();
	~Printer();
	static void safe_print(std::string);
};

