#include "CodecsConfig.hpp"
#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>

const std::string CODECSCONFIGFILE = "./config/codecs_config.json";
long CodecsConfig::_fileSizeInChars;
char *CodecsConfig::_buffer;
std::string CodecsConfig::_jsonContent;
Json::Value CodecsConfig::_data;
unsigned int CodecsConfig::_outputFps;
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

	if(configFile == NULL)
	{
		std::cout << "Error while opening '" << CODECSCONFIGFILE << "'"
				  << std::endl;
		exit(-1);
	}

	setFileSizeInChars(configFile);
	loadFileToBuffer(configFile);

	CodecsConfig::_jsonContent = _buffer;

	fclose(configFile);
	free(_buffer);
}

void CodecsConfig::setFileSizeInChars(FILE *file)
{
	fseek(file, 0, SEEK_END);
	_fileSizeInChars = ftell(file);
	rewind(file);
}

void CodecsConfig::loadFileToBuffer(FILE *file)
{
	_buffer = (char *)malloc(sizeof(char) * (_fileSizeInChars + 1));

	if(_buffer == NULL)
	{
		std::cout << "Could not load file '" << CODECSCONFIGFILE
				  << "' to memory" << std::endl;
		exit(-2);
	}

	// Loading file to buffer
	size_t r = fread(_buffer, sizeof(char), _fileSizeInChars, file);

	if(r != _fileSizeInChars)
		exit(-2);
	_buffer[_fileSizeInChars] = '\0';
}

void CodecsConfig::parseJsonContent()
{
	Json::Reader reader;
	bool wellFormed;

	wellFormed = reader.parse(_jsonContent, _data, false);

	if(!wellFormed)
	{
		std::cout << "Failed to parse JSON" << std::endl
				  << reader.getFormattedErrorMessages() << std::endl;
		exit(-3);
	}
}

void CodecsConfig::fillCodecsMap()
{
	for(unsigned int i = 0; i < _data.size() - 1; i++)
	{
		std::tuple<std::string, std::string> tempTuple(
			_data[i]["codecFourcc"].asString(),
			_data[i]["fileExtension"].asString());

		codecs[_data[i]["codecName"].asString()] = tempTuple;
	}

	_outputFps = _data[_data.size() - 1]["outputFPS"].asUInt();

	_buffer = NULL;
}

void CodecsConfig::listCodecs()
{
	std::map<std::string, std::tuple<std::string, std::string>>::iterator it;

	std::cout << "---------------------"
				 "---------------------"
				 "----------"
			  << std::endl;

	for(it = codecs.begin(); it != codecs.end(); it++)
	{
		std::cout << "Codec Name: " << it->first << std::endl;
		std::cout << "FourCC code: " << std::get<0>(it->second) << std::endl;
		std::cout << "File extension: " << std::get<1>(it->second) << std::endl;
		std::cout << "-----------------"
					 "-----------------"
					 "-----------------"
					 "-"
				  << std::endl;
	}
}

int CodecsConfig::getCodecFourcc(std::string codecName)
{
	if(codecs.size() == 0)
	{
		std::cout << "'" << CODECSCONFIGFILE << "' has not been read "
												"yet."
				  << std::endl;
		exit(-5);
	}

	auto codec = codecs.find(codecName);

	if(codec == codecs.end())
		return -4;

	const char *fourccChar = std::get<0>(codec->second).c_str();

	return cv::VideoWriter::fourcc(fourccChar[0], fourccChar[1], fourccChar[2],
								   fourccChar[3]);
}

std::string CodecsConfig::getCodecExtension(std::string codecName)
{
	if(codecs.size() == 0)
	{
		std::cout << "'" << CODECSCONFIGFILE << "' has not been read "
												"yet."
				  << std::endl;
		exit(-5);
	}

	auto codec = codecs.find(codecName);

	return codec == codecs.end() ? "" : std::get<1>(codec->second);
}

