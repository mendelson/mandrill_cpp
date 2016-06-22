#pragma once

#include <map>
#include <string>
#include <tuple>
#include "json/json.h"

class CodecsConfig
{
public:
	static void update();

	// Returns -4 if the codec has not been found
	static int getCodecFourcc(std::string codecName);

	// Returns an empty string if the codec has not been found
	static std::string getCodecExtension(std::string codecName);

private:
	static void readJsonFile();
	static void setFileSizeInChars(FILE *file);
	static void loadFileToBuffer(FILE *file);
	static void parseJsonContent();
	static void fillCodecsMap();
	static void listCodecs();

	static long fileSizeInChars;
	static char *buffer;
	static std::string jsonContent;
	static Json::Value data;

	/* Key: codecName
	*  Tuple:
	*  	- First element: codecFourcc
	*	- Second element: fileExtension
	*/
	static std::map<std::string, std::tuple<std::string, std::string>> codecs;
};

