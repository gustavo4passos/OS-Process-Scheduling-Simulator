#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H

#include <vector>
#include <set>
#include "proccess.h"
#include "scheduler.h"

class Memory;
class Proccess;
class Scheduler;

class OperatingSystem
{
public:
    OperatingSystem(
        int quantum, 
        int overhead, 
        SchedulingAlgorithm schedulingAlgorithm
    );

    inline unsigned int GetQuantum()        const { return m_quantum; }
    inline unsigned int GetOverhead()       const { return m_overhead; }
    inline unsigned GetNumberOfProccesses() const { m_numberOfProccesses; }

    float GetAverageTurnaround() const;

    const std::vector<Proccess*>& GetFinishedProccesses() const { return m_finishedProccesses; }
    const std::vector<Proccess*>& GetActiveProccesses()   const { return m_executionQueue; }

    void AddProccess(Proccess* proccess);

    // Simulates for one time unit.
    // If there are still steps to be taken, return true.
    // Otherwise, returns false.
    bool NextStep();

private:
    unsigned int m_quantum;
    unsigned int m_overhead;
    Scheduler* m_scheduler;
    Memory* m_virtualMemory;
    Memory* m_RAMMemory;
    unsigned int m_time;
    unsigned m_numberOfProccesses;

    std::vector<Proccess*> m_executionQueue;
    std::vector<Proccess*> m_proccesses;
    std::vector<Proccess*> m_finishedProccesses;

    void UpdateExecutionQueue();
};

#endif