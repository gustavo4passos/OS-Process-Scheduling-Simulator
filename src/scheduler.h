#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>

class Proccess;

enum class SchedulingAlgorithm
{
    FIFO,
    SJF,
    ROUND_ROBIN,
    EDF
};

class Scheduler 
{
public:
    Scheduler(
        int quantum, 
        int overload, 
        SchedulingAlgorithm algorithm
    ) { }

    inline int Quantum()  { return m_quantum;  }
    inline int Overload() { return m_overload; }

    void Run(std::vector<Proccess*>* executionQueue);
    void SetSchedulingAlgorithm(SchedulingAlgorithm shedulingAlgorithm) { }

private:
    int m_quantum;
    int m_overload;
    SchedulingAlgorithm m_algorithm;

    // The FIFO algorithm assumes that execution queue is alread
    // ordered by arrival time.
    void RunFIFO(std::vector<Proccess*>* executionQueue);
    void RunSJF(std::vector<Proccess*>* executionQueue);
    void RunRoundRobin(std::vector<Proccess*>* executionQueue);
    void RunEDF(std::vector<Proccess*>* executionQueue);
};

#endif