/* Universidade Federal da Bahia         */
/* Simulador de Escalonador de Processos */
/* Gustavo Passos                        */
/* Ana Clara Batista                     */
/* Joao Victor                           */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>

class Proccess;
class MemoryManager;

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
        unsigned quantum, 
        unsigned overhead, 
        SchedulingAlgorithm algorithm
    );

    inline unsigned GetQuantum()  const { return m_quantum;  }
    inline unsigned GetOverhead() const { return m_overhead; }

    void Run(
        std::vector<Proccess*>* executionQueue,
        std::vector<Proccess*>* finishedProccesses,
        std::vector<Proccess*>* blockedProcesses,
        MemoryManager* MemoryManager,
        unsigned time
    );

    // Reset timers
    void Reset();
    
    SchedulingAlgorithm GetSchedulingAlgorithm() const { return m_algorithm; }
    // Changes the current scheduling algoritm.
    // Attention: All the proccesses in the execution queue
    // will be set to ready.
    void SetSchedulingAlgorithm(
        SchedulingAlgorithm shedulingAlgorithm,
        std::vector<Proccess*>* executionQueue
        );

private:
    unsigned m_quantum;
    unsigned m_overhead;
    SchedulingAlgorithm m_algorithm;

    unsigned m_currentProccessTimeLeft;
    unsigned m_overheadTimeLeft;;

    // The FIFO algorithm assumes that execution queue is already
    // ordered by arrival time.
    void RunFIFO(
        std::vector<Proccess*>* executionQueue,
        std::vector<Proccess*>* finishedProccesses,
        std::vector<Proccess*>* blockedProcesses,
        MemoryManager* MemoryManager,
        unsigned time
        );
    void RunSJF(
        std::vector<Proccess*>* executionQueue,
        std::vector<Proccess*>* finishedProccesses,
        std::vector<Proccess*>* blockedProcesses,
        MemoryManager* MemoryManager,
        unsigned time
        );
    void RunRoundRobin(
        std::vector<Proccess*>* executionQueue,
        std::vector<Proccess*>* finishedProccesses,
        std::vector<Proccess*>* blockedProcesses,
        MemoryManager* MemoryManager,
        unsigned time
        );
    void RunEDF(
        std::vector<Proccess*>* executionQueue,
        std::vector<Proccess*>* finishedProccesses,
        std::vector<Proccess*>* blockedProcesses,
        MemoryManager* MemoryManager,
        unsigned time
    );

    void MoveShortestJobToTheFront(
        std::vector<Proccess*>* executionQueue
    );
    void MoveCurrentProccessToFinishedList(
        std::vector<Proccess*>* executionQueue,
        std::vector<Proccess*>* finishedProccesses
    );
    void MoveCurrentProccessToEndOfQueue(
        std::vector<Proccess*>* executionQueue
    );
    void MoveProccessWithEarliestDeadline(
        std::vector<Proccess*>* executionQueue
    );
};

#endif