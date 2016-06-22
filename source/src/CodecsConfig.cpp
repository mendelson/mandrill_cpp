#include "CodecsConfig.hpp"
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>

const std::string CODECSCONFIGFILE = "./config/codecs_config.json";
long CodecsConfig::fileSizeInChars;
char *CodecsConfig::buffer;
std::string CodecsConfig::jsonContent;
Json::Value CodecsConfig::data;
std::map<std::string, std::tuple<std::string, std::string>> CodecsConfig::codecs;

void CodecsConfig::update()
{
	readJsonFile();
	parseJsonContent();
	fillCodecsMap();
	// listCodecs();
}

void CodecsConfig::readJsonFile()
{
	FILE *configFile = nullptr;

	configFile = fopen(CODECSCONFIGFILE.c_str(), "rb");

	if (configFile == NULL)
	{
		std::cout << "Error while opening '" << CODECSCONFIGFILE << "'"
				  << std::endl;
		exit(-1);
	}

	setFileSizeInChars(configFile);
	loadFileToBuffer(configFile);

	CodecsConfig::jsonContent = buffer;

	fclose(configFile);
	free(buffer);
}

void CodecsConfig::setFileSizeInChars(FILE *file)
{
	fseek(file, 0, SEEK_END);
	fileSizeInChars = ftell(file);
	rewind(file);
}

void CodecsConfig::loadFileToBuffer(FILE *file)
{
	buffer = (char *)malloc(sizeof(char) * (fileSizeInChars + 1));

	if (buffer == NULL)
	{
		std::cout << "Could not load file '" << CODECSCONFIGFILE
				  << "' to memory" << std::endl;
		exit(-2);
	}

	// Loading file to buffer
	fread(buffer, sizeof(char), fileSizeInChars, file);
	buffer[fileSizeInChars] = '\0';
}

void CodecsConfig::parseJsonContent()
{
	Json::Reader reader;
	bool wellFormed;

	wellFormed = reader.parse(jsonContent, data, false);

	if (!wellFormed)
	{
		std::cout << "Failed to parse JSON" << std::endl
				  << reader.getFormattedErrorMessages() << std::endl;
		exit(-3);
	}
}

void CodecsConfig::fillCodecsMap()
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		std::tuple<std::string, std::string> tempTuple(
			data[i]["codecFourcc"].asString(),
			data[i]["fileExtension"].asString());

		codecs[data[i]["codecName"].asString()] = tempTuple;
	}

	buffer = NULL;
}

void CodecsConfig::listCodecs()
{
	std::map<std::string, std::tuple<std::string, std::string>>::iterator it;

	std::cout << "----------------------------------------------------"
			  << std::endl;

	for (it = codecs.begin(); it != codecs.end(); it++)
	{
		std::cout << "Codec Name: " << it->first << std::endl;
		std::cout << "FourCC code: " << std::get<0>(it->second) << std::endl;
		std::cout << "File extension: " << std::get<1>(it->second) << std::endl;
		std::cout << "----------------------------------------------------"
				  << std::endl;
	}
}

int CodecsConfig::getCodecFourcc(std::string codecName)
{
	if (codecs.size() == 0)
	{
		std::cout << "'" << CODECSCONFIGFILE << "' has not been read yet."
				  << std::endl;
		exit(-5);
	}

	auto codec = codecs.find(codecName);

	if (codec == codecs.end())
		return -4;

	const char *fourccChar = std::get<0>(codec->second).c_str();

	return cv::VideoWriter::fourcc(fourccChar[0], fourccChar[1], fourccChar[2],
								   fourccChar[3]);
}

std::string CodecsConfig::getCodecExtension(std::string codecName)
{
	if (codecs.size() == 0)
	{
		std::cout << "'" << CODECSCONFIGFILE << "' has not been read yet."
				  << std::endl;
		exit(-5);
	}

	auto codec = codecs.find(codecName);

	return codec == codecs.end() ? "" : std::get<1>(codec->second);
}

