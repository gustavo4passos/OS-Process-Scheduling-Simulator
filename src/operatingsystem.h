#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H

#include <vector>
#include "operatingsystem.h"
#include "proccess.h"
#include "scheduler.h"

class Memory;

class OperatingSystem
{
public:
    OperatingSystem(
        int quantum, 
        int overhead, 
        SchedulingAlgorithm schedulingAlgorithm
    );
    ~OperatingSystem();

    inline unsigned int GetQuantum()        const { return m_quantum; }
    inline unsigned int GetOverhead()       const { return m_overhead; }
    inline unsigned GetNumberOfProccesses() const { return m_numberOfProccesses; }

    float GetAverageTurnaround() const;
    unsigned GetExecutionTime() const { return m_time; }

    const std::vector<Proccess*>& GetFinishedProccesses() const { return m_finishedProccesses; }
    const std::vector<Proccess*>& GetActiveProccesses()   const { return m_executionQueue;     }

    // Add a proccess to the proccess list.
    void AddProccess(int id, unsigned arrivalTime, unsigned duration, unsigned deadline);

    // Simulates for one time unit.
    // If there are still steps to be taken, return true.
    // Otherwise, returns false.
    bool NextStep();

    // Reset simulation to it's initial state
    void Reset();

    // Changes the current scheduling algorithm.
    // Attention: All proccesses in the execution
    // queue will be set to idle and the next
    // proccess to be executed will be decided
    // based on the new scheduling algorithm
    void ChangeSchedulingAlgorithm(SchedulingAlgorithm newAlgorithm);

private:
    unsigned m_quantum;
    unsigned m_overhead;
    Scheduler* m_scheduler;
    Memory* m_virtualMemory;
    Memory* m_RAMMemory;
    unsigned m_time;
    unsigned m_numberOfProccesses;

    std::vector<Proccess*> m_executionQueue;
    std::vector<Proccess*> m_proccesses;
    std::vector<Proccess*> m_finishedProccesses;

    // Add arriving proccess to the execution queue
    void UpdateExecutionQueue();

};

#endif