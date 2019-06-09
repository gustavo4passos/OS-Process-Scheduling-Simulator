#include "memorymanager.h"
#include <algorithm>
#include <assert.h>

#define M_DEBUG

MemoryManager::MemoryManager(
    PageReplacementAlgorithm algorithm)
:   m_nextAvailableVirtualPage(0),
    m_pageReplacementAlgorithm(algorithm),
    m_counter(0)
{
    // Initialize all blocks as free
    for(auto i = 0; i < 50; ++i)
    {
        RAM.push_back(-1);
        m_freeRamPages.push(i);
    }
}

bool MemoryManager::AreAllPagesInMemory(
    const std::vector<unsigned>& pages)
{
    for(auto& p : pages)
    {
        #ifdef M_DEBUG
        assert(p < m_pageTable.size());
        #endif

        if(!m_pageTable[p].present) return false;
    }

    return true;
}

int MemoryManager::LoadAbsentPagesToMemory(
    const std::vector<unsigned>& pages)
{
    int loadsAvailableForThisTick = PAGES_LOADED_PER_TICK;
    
    std::vector<int> pagesToLoad;

    auto p = pages.begin();
    while(p != pages.end() && loadsAvailableForThisTick > 0)
    {
        if(m_pageTable[*p].present == false)
        {
            pagesToLoad.push_back(*p);
            loadsAvailableForThisTick--;
        }

        p++;
    }

    if (pagesToLoad.size() < 1) return 0;

    for(auto& p : pagesToLoad)
    {  
        int freePage = -1;

        // If there are free pages in RAM, load absent page there
        if(m_freeRamPages.size() == 0)
        {
            if(m_pageReplacementAlgorithm == PageReplacementAlgorithm::LRU)
            {
                freePage = GetLeastRecentlyUsedPage();
            }
            else // FIFO
            {
                int oldestPage = GetOldestPage();
                freePage = m_pageTable[oldestPage].pageFrameNumber;
                DeletePage(oldestPage);
            }
        }

        freePage = m_freeRamPages.front();
        m_freeRamPages.pop();

        #ifdef M_DEBUG
        assert(freePage >= 0);
        #endif

        LoadPage(p, freePage);
    }

    return pagesToLoad.size();
}

std::vector<unsigned> MemoryManager::ReserveVirtualPages(int howMany)
{
    std::vector<unsigned> reservedPages;

    #ifdef M_DEBUG
    assert(howMany > 0);
    #endif
    
    for(int i = 0; i < howMany; i++)
    {
        reservedPages.push_back(m_nextAvailableVirtualPage);
        m_nextAvailableVirtualPage++; 
    }

    if(m_nextAvailableVirtualPage > m_pageTable.size())
    {
        GrowPageTable(m_nextAvailableVirtualPage - m_pageTable.size());
    }

    return reservedPages;
}

void MemoryManager::ProtectPages(const std::vector<unsigned>& pages)
{
    for(auto& page : pages)
    {
        ProtectPage(page);
    }
}

void MemoryManager::RemoveProtectionFromPages(
    const std::vector<unsigned>& pages)
{

    for(auto& p : pages)
    {
        #ifdef M_DEBUG
        assert((unsigned)p < m_pageTable.size());
        #endif
        UnprotectPage(p);
    }
}

void MemoryManager::DeletePages(const std::vector<unsigned>& pages)
{
    for(auto& p : pages)
    {
        DeletePage(p);
    }
}

void MemoryManager::LoadPage(unsigned page, unsigned ramPage)
{
    #ifdef M_DEBUG
    assert((unsigned)page < m_pageTable.size());
    #endif

    RAM[ramPage] = page;
    m_pageTable[page].present = true;
    m_pageTable[page].lastReferenceTime = m_counter;
    m_pageTable[page].pageFrameNumber = ramPage;
    // m_pageTable[page].protection = true;

    if(m_pageReplacementAlgorithm == PageReplacementAlgorithm::FIFO)
    {
        m_loadedPages.push_back(page);
    }
}

void MemoryManager::DeletePage(unsigned page)
{
    #ifdef M_DEBUG
    assert((unsigned)page < m_pageTable.size());
    #endif

    m_pageTable[page].protection = false;

    if(m_pageTable[page].present)
    {
        // Set RAM page as free
        RAM[m_pageTable[page].pageFrameNumber] = -1;
        m_freeRamPages.push(m_pageTable[page].pageFrameNumber);
    }
    m_pageTable[page].present = false;
    m_pageTable[page].pageFrameNumber = -1;
}

void MemoryManager::ProtectPage(int page)
{
    #ifdef M_DEBUG
    assert((unsigned)page < m_pageTable.size());
    #endif

    m_pageTable[page].protection = false;
}

void MemoryManager::UnprotectPage(int page)
{
    #ifdef M_DEBUG
    assert((unsigned)page < m_pageTable.size());
    #endif

    m_pageTable[page].protection = true;
}


void MemoryManager::GrowPageTable(unsigned amount)
{
    #ifdef M_DEBUG
    assert(amount > 0);
    #endif

    for(unsigned i = 0; i < amount; i++)
        m_pageTable.push_back( { false, false, false, 0, 0, false } );
}


int MemoryManager::GetOldestPage()
{
    #ifdef M_DEBUG
    // Asserts there's at least one loaded page
    assert(m_loadedPages.size() > 0);
    #endif

    int oldestPage = -1;
    for(auto p = m_loadedPages.begin(); p != m_loadedPages.end();)
    {
        // If page is absent, remove it from the list
        if(!m_pageTable[*p].present) m_loadedPages.erase(p);

        else if(m_pageTable[*p].protection) p++;
        else // Found not protected present oldest page
        {
            oldestPage = *p;
            break;
        }
    }
 
    #ifdef M_DEBUG
    // Asserts oldest page has been found
    assert(oldestPage >= 0);
    #endif

    return oldestPage;
}

int MemoryManager::GetLeastRecentlyUsedPage()
{
    int oldestPage = 0;

    for(int i = 0; i < m_pageTable.size(); i++)
    {
        if(m_pageTable[i].lastReferenceTime < m_pageTable[i].lastReferenceTime &&
           m_pageTable[i].present)
        {
            oldestPage = i;
        }
    }
    return oldestPage;
}