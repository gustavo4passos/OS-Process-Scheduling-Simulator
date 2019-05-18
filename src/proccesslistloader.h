#ifndef PROCCESSLISTLOADER_H
#define PROCCESSLISTLOADER_H

#include <string>
#include "proccesslist.h"

namespace ProccessListLoader 
{
    void SaveProccessList(
        const std::string& filename, 
        const std::vector<ProccessTemplate>& proccessList
    );

    std::vector<ProccessTemplate> LoadProccessList(const std::string& filename);
}

#endif