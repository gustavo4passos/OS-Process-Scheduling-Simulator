#include "proccesslistloader.h"
#include <fstream>
#include <string.h>

namespace ProccessListLoader
{
void SaveProccessList(
	const std::string& filename, 
	const std::vector<ProccessTemplate>& proccessList)
{
	if(proccessList.empty()) return;
	if(filename == "") return;

	std::fstream file;
	file.open(filename, std::fstream::binary | std::fstream::out);

	if(!file.is_open()) return;

	// Write header
	file.write((const char*)&PLFFILEHEADER[0], SIZEOFPLFFILEHEADER);

	for(auto proccess = proccessList.begin();
		proccess != proccessList.end();
		proccess++)
	{
		file.write((const char*)(&(*proccess).ID), sizeof(unsigned));
		file.write((const char*)(&(*proccess).arrivalTime), sizeof(unsigned));
		file.write((const char*)(&(*proccess).duration), sizeof(unsigned));
		file.write((const char*)(&(*proccess).deadline), sizeof(unsigned));
	}

	file.close();
}


std::vector<ProccessTemplate> LoadProccessList(const std::string& filename)
{
	std::vector<ProccessTemplate> proccessList;
	std::fstream file;
	file.open(filename, std::fstream::binary | std::fstream::in);
	if(!file.is_open()) return proccessList; 

	// Check header
	char header[SIZEOFPLFFILEHEADER];
	file.read((char*)header, SIZEOFPLFFILEHEADER);
	if(strcmp(header, PLFFILEHEADER) != 0)
	{
		return proccessList;
	}

	// For each field, read from the file and check if the stream
	// is still good
	while(file.peek() != EOF)
	{
		unsigned ID, duration, arrivalTime, deadline;		 

		file.read((char*)&ID, sizeof(unsigned));
		if(!file.good()) return std::vector<ProccessTemplate>();

		file.read((char*)&arrivalTime, sizeof(unsigned));
		if(!file.good()) return std::vector<ProccessTemplate>();

		file.read((char*)&duration, sizeof(unsigned));
		if(!file.good()) return std::vector<ProccessTemplate>();

		file.read((char*)&deadline, sizeof(unsigned));
		if(!file.good()) return std::vector<ProccessTemplate>();

		proccessList.push_back({ ID, duration, arrivalTime, deadline });
	}

	file.close();

	//If all proccess weres successfully read, return proccessList
	return proccessList;
}
}

