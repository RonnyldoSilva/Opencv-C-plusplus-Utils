/*
 * InputOutput.cpp
 *
 *  Created on: Jul 4, 2017
 *      Author: carlos
 */

#include "InputOutput.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <boost/property_tree/ptree.hpp>
#include <utility>
#include <boost/property_tree/json_parser.hpp>
#include <dirent.h>

InputOutput::InputOutput() {
	// TODO Auto-generated constructor stub

}

std::map<std::string, std::string> InputOutput::readJson(std::string filename)
{
	std::map<std::string, std::string> result;

	std::ifstream jsonFile(filename.c_str());

	boost::property_tree::ptree pt;

	boost::property_tree::read_json(jsonFile, pt);

	boost::property_tree::ptree classificador =	pt.get_child("classificador");
	for (boost::property_tree::ptree::const_iterator it = classificador.begin(); it != classificador.end(); ++it)
	{
		result[it->first] = classificador.get<std::string>(it->first);
	}

	return result;
}

std::map<int, std::string> InputOutput::readIndexLabelFile(std::string filename)
{
	std::map<int, std::string> indexLabel;
	std::ifstream fin(filename.c_str());

	if(fin.fail())
	{
		std::cout<<"Did not open file: "<<filename<<std::endl;
		return indexLabel;
	}

	std::string line;
	std::vector<std::string> aux;

	while(fin)
	{
		getline(fin, line);
		aux.clear();
		if(!line.empty())
		{
			tokenize(line,aux, " ");
			std::string sIndex = aux[1];
			int index = atoi(sIndex.c_str());
			indexLabel[ index ] = aux[0];
		}
	}

	return indexLabel;
}


std::map<char, char> InputOutput::read_dict(std::string filename)
{
	std::map<char, char> result;
	std::ifstream fin;
	fin.open(filename.c_str());

	std::string line;
	while(fin)
	{
		getline(fin, line);
		if(line.length() < 3) continue;
		result[line[0]] = line[2];
	}

	return result;
}

std::vector<std::string> InputOutput::read_ocr_result(std::string filename)
{
	std::vector<std::string> result;

	std::ifstream in;
	in.open(filename.c_str());

	while(in)
	{
		std::string line;
		getline(in, line);
		result.push_back(line);
	}

	return result;
}

std::map<std::string, int> InputOutput::convertIndexLabelFile(std::map<int, std::string> indexLabel){

	std::map<std::string, int> result;

	for (int i = 0; i < indexLabel.size(); ++i)
	{
		std::string label = indexLabel[i];
		result[label] = i;
	}
	return result;
}

void InputOutput::tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters)
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos  = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

std::vector<std::string> InputOutput::getName(std::string pathDir)
{
	struct dirent **namelist;
	int n;
	std::vector<std::string> fileNames;
	n = scandir(pathDir.c_str(), &namelist, 0, versionsort);
	if (n < 0)
	{
		std::cout<<"\n scandir error \n";
		return fileNames;

	}
	else
	{
		int i = 0;
		while( i < n)
		{
			if((i!=0) && (i!=1))
			{
				fileNames.push_back(namelist[i]->d_name);
			}

			free(namelist[i]);
			i++;
		}
		free(namelist);
	}
	return fileNames;
}

InputOutput::~InputOutput() {
	// TODO Auto-generated destructor stub
}
