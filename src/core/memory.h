#ifndef MEMORY_H
#define MEMORY_H

#include <vector>

struct PageEntry
{
    bool present;
    bool referenced;
    bool modified;
    unsigned pageFrameNumber;
    unsigned lastReferenceTime;
    bool protection;

    bool operator<(const PageEntry& rhs)
    {
        return lastReferenceTime < rhs.lastReferenceTime;
    }
};

class Memory
{
public:
    explicit Memory(unsigned numberOfPages);
    void Grow(unsigned amount);
    const PageEntry& GetPageEntry(int page) const;

    void ProtectPage(int page);
    void UnprotectPage(int page);

    const unsigned GetSize() { return m_size; }

private:
    std::vector<PageEntry> m_pageTable;
    unsigned m_size;
};

#endif