#include "proccesslistloader.h"
#include <fstream>
#include <iostream>

#include <iostream>
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

        for(auto proccess = proccessList.begin();
            proccess != proccessList.end();
            proccess++)
        {
            file.write((const char*)(&(*proccess).ID), sizeof(unsigned));
            file.write((const char*)(&(*proccess).duration), sizeof(unsigned));
            file.write((const char*)(&(*proccess).arrivalTime), sizeof(unsigned));
            file.write((const char*)(&(*proccess).deadline), sizeof(unsigned));
        }

        file.close();
    }
}