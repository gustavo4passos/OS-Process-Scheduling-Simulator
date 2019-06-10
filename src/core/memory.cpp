#include "memory.h"
#include "assert.h"

Memory::Memory(unsigned numberOfPages)
{
    for(unsigned i = 0; i < numberOfPages * 2; i++)
    {
        m_pageTable.push_back( { false, false, false, 0, 0, false } );
    }
}

const PageEntry& Memory::GetPageEntry(int page) const {
    assert(page < m_pageTable.size());

    return m_pageTable[page];
}

void Memory::Grow(unsigned amount)
{
    if(amount < 1) return;

    for(unsigned i = 0; i < amount; i++)
        m_pageTable.push_back( { false, false, false, 0, 0, false } );
}

void Memory::ProtectPage(int page)
{
    assert(page < m_pageTable.size());

    m_pageTable[page].protection = false;
}

void Memory::UnprotectPage(int page)
{
    assert(page < m_pageTable.size());

    m_pageTable[page].protection = true;
}