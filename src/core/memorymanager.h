#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <queue>

enum class PageReplacementAlgorithm
{
    FIFO,
    LRU
};

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

class MemoryManager
{
public:
    explicit MemoryManager(PageReplacementAlgorithm algorithm);

    bool AreAllPagesInMemory(const std::vector<unsigned>& pages);
    int  LoadAbsentPagesToMemory(const std::vector<unsigned>& pages);
    int  ClearPagesFromMemory(const std::vector<unsigned>& pages);
    void ProtectPages(const std::vector<unsigned>& pages);
    void RemoveProtectionFromPages(const std::vector<unsigned>& pages);
    void DeletePages(const std::vector<unsigned>& pages);
    void ReferencePages(const std::vector<unsigned>& pages);
    
    unsigned GetNumberOfFreeRamPages() { return m_freeRamPages.size(); }
    PageReplacementAlgorithm GetPageReplacementAlgorithm() const
    { 
        return m_pageReplacementAlgorithm; 
    }
    
    const std::vector<int>& GetRAM() const { return RAM; }

    // Increase counter by 1
    void Tick() { m_counter++; }
    std::vector<unsigned> ReserveVirtualPages(int howMany);

    friend class OperatingSystem;
    
private:
    unsigned m_nextAvailableVirtualPage;
    PageReplacementAlgorithm m_pageReplacementAlgorithm;

    // Page table
    void LoadPage(unsigned page, unsigned ramPage);
    void DeletePage(unsigned page);
    void ProtectPage(int page);
    void UnprotectPage(int page);
    void GrowPageTable(unsigned amount);

    int GetOldestPage();
    int GetLeastRecentlyUsedPage();
    
    std::vector<PageEntry> m_pageTable;
    std::vector<int> m_loadedPages;

    static constexpr unsigned RAM_SIZE = 50;
    std::vector<int> RAM;
    std::queue<int> m_freeRamPages;

    static constexpr unsigned PAGES_LOADED_PER_TICK = 5;
    unsigned m_counter;
};

#endif 