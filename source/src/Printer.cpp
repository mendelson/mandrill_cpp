#include <iostream>
#include <mutex>
#include "Printer.hpp"

extern std::mutex printer_mutex;


void Printer::safe_print(std::string Msg)
{
	#ifdef DEBUG
		printer_mutex.lock();
	#endif	
		
	std::cout << Msg << std::endl;

	#ifdef DEBUG
		printer_mutex.unlock();
	#endif	

}