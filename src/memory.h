#ifndef MEMORY_H
#define MEMORY_H

#include <vector>

struct Page
{
    bool modified;
    unsigned pageFrameNumber;
};

class Memory
{
public:
    Memory(int numberOfPages);

private:
    std::vector<Page> m_pages;
};

#endif