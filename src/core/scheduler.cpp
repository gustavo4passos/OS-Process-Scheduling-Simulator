#include "scheduler.h"
#include "memorymanager.h"
#include "proccess.h"
#include <assert.h>

Scheduler::Scheduler(
        unsigned quantum, 
        unsigned overhead, 
        SchedulingAlgorithm algorithm)
:   m_quantum(quantum),
    m_overhead(overhead),
    m_algorithm(algorithm),
    m_currentProccessTimeLeft(0),
    m_overheadTimeLeft(0)
{ }

void Scheduler::Run(
    std::vector<Proccess*>* executionQueue,
    std::vector<Proccess*>* finishedProccesses, 
    std::vector<Proccess*>* blockedProccesses,
    MemoryManager* memoryManager,
    unsigned time)
{
    switch(m_algorithm)
    {
        case SchedulingAlgorithm::FIFO:
            RunFIFO(
                executionQueue, 
                finishedProccesses,
                blockedProccesses,
                memoryManager,
                time);
            break;
        
        case SchedulingAlgorithm::SJF:
            RunSJF(
                executionQueue, 
                finishedProccesses,
                blockedProccesses,
                memoryManager,
                time);
            break;
        
        case SchedulingAlgorithm::ROUND_ROBIN:
            RunRoundRobin(
                executionQueue, 
                finishedProccesses,
                blockedProccesses,
                memoryManager,
                time);
            break;

        case SchedulingAlgorithm::EDF:
            RunEDF(
                executionQueue, 
                finishedProccesses,
                blockedProccesses,
                memoryManager,
                time);
            break;
        
        default:
            break;
    }
}

void Scheduler::Reset()
{
    m_currentProccessTimeLeft = 0;
    m_overheadTimeLeft = 0;
}

void Scheduler::SetSchedulingAlgorithm(
    SchedulingAlgorithm algorithm,
    std::vector<Proccess*>* executionQueue)
{
    m_currentProccessTimeLeft = 0;
    m_overheadTimeLeft = 0;
    m_algorithm = algorithm;

    for(auto p = executionQueue->begin(); p != executionQueue->end(); p++)
    {
        (*p)->SetState(ProccessState::READY);
    }
}

void Scheduler::RunFIFO(
    std::vector<Proccess*>* executionQueue,
    std::vector<Proccess*>* finishedProccesses,
    std::vector<Proccess*>* blockedProcesses,
    MemoryManager* memoryManager,
    unsigned time)
{
    #ifdef M_DEBUG
    assert(executionQueue     != nullptr &&
           finishedProccesses != nullptr &&
           blockedProcesses   != nullptr &&
           memoryManager      != nullptr);
    #endif

    if(executionQueue->empty() && blockedProcesses->empty()) return;

    if(executionQueue->front()->GetState() == ProccessState::DONE)
    {
        // Remove proccess pages from RAM
        memoryManager->DeletePages(executionQueue->front()->GetPages());
        MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
    }

    if(!executionQueue->empty())
    {
        Proccess* currentProccess = executionQueue->front();

        // Check if current proccess has all pages in memory
        if(memoryManager->AreAllPagesInMemory(currentProccess->GetPages()))
        {
            if(currentProccess->GetState() == ProccessState::BLOCKED)
                currentProccess->SetState(ProccessState::RUNNING);

            executionQueue->front()->Run(time);
        }
        else // If not, load as many pages as it's possible in one cicle
        {
            currentProccess->SetState(ProccessState::BLOCKED);
            memoryManager->LoadAbsentPagesToMemory(currentProccess->GetPages());
        }
    }
}

void Scheduler::RunSJF(
    std::vector<Proccess*>* executionQueue,
    std::vector<Proccess*>* finishedProccesses,
    std::vector<Proccess*>* blockedProcesses,
    MemoryManager* memoryManager,
    unsigned time)
{
    #ifdef M_DEBUG
    assert(executionQueue     != nullptr &&
           finishedProccesses != nullptr &&
           blockedProcesses   != nullptr &&
           memoryManager      != nullptr);
    #endif

    // Check if there are active proccesses
    if(executionQueue->empty()) return;

    // Current proccess has finished, and needs to be
    // removed from execution queue and put into 
    // the finished proccesses list.
    if(executionQueue->front()->GetState() == ProccessState::DONE)
    {
        MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
        // If there are no more proccesses to be run,
        // return.
        if(executionQueue->empty()) return;
    }

    // If there are no proccesses running atm,
    // find the next proccess to be run, insert
    // it at the front of the queue.
    if(executionQueue->front()->GetState() == ProccessState::READY)
    {
        MoveShortestJobToTheFront(executionQueue);
    }

    Proccess* currentProccess = executionQueue->front();
    
    if(memoryManager->AreAllPagesInMemory(currentProccess->GetPages()))
    {
        if(currentProccess->GetState() == ProccessState::BLOCKED)
        {
            currentProccess->SetState(ProccessState::RUNNING);
        }
        executionQueue->front()->Run(time);
    }
    else
    {
        currentProccess->SetState(ProccessState::BLOCKED);
        memoryManager->LoadAbsentPagesToMemory(currentProccess->GetPages());
    }
    
}

void Scheduler::RunRoundRobin(
    std::vector<Proccess*>* executionQueue,
    std::vector<Proccess*>* finishedProccesses,
    std::vector<Proccess*>* blockedProcesses,
    MemoryManager* memoryManager,
    unsigned time)
{
    #ifdef M_DEBUG
    assert(executionQueue     != nullptr &&
           finishedProccesses != nullptr &&
           blockedProcesses   != nullptr &&
           memoryManager      != nullptr);
    #endif
    if(executionQueue->empty()) return;

    // Current proccess has used all it's available quantum
    // OR the first proccess is about to start.
    if(m_currentProccessTimeLeft == 0)
    {
        // Execution is about to start
        if(executionQueue->front()->GetState() == ProccessState::READY)
        {
            m_currentProccessTimeLeft = m_quantum - 1;
            memoryManager->ProtectPages(executionQueue->front()->GetPages());
        }
        // Current proccess has finished executing
        else if(executionQueue->front()->GetState() == ProccessState::DONE)
        {
            // Clear the pages of the current process from memory
            memoryManager->ClearPagesFromMemory(executionQueue->front()->GetPages());
            MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
            
            if(!executionQueue->empty()) m_currentProccessTimeLeft = m_quantum - 1;
        }
        else if(executionQueue->front()->GetState() == ProccessState::RUNNING)
        {
            executionQueue->front()->SetState(ProccessState::OVERHEAD);
            m_overheadTimeLeft = m_overhead - 1;
        }
        // Proccess state is being saved (overhead)
        else
        {
            if(m_overheadTimeLeft > 0)
            {
                m_overheadTimeLeft--;
            }
            else
            {
                m_overheadTimeLeft = 0;
                executionQueue->front()->SetState(ProccessState::READY);
                if(executionQueue->size() > 1)
                {
                    MoveCurrentProccessToEndOfQueue(executionQueue);
                }
                m_currentProccessTimeLeft = m_quantum - 1;
            }
        }
    }
    else 
    {
        if(executionQueue->front()->GetState() == ProccessState::DONE)
        {
            MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
            if(!executionQueue->empty()) m_currentProccessTimeLeft = m_quantum - 1;

            // There are no more proccesses in the execution queue
            else m_currentProccessTimeLeft = 0;
        }
        else
        {
            m_currentProccessTimeLeft--;
        }  
    }

    if(executionQueue->size() > 0) executionQueue->front()->Run(time);
}

void Scheduler::RunEDF(
    std::vector<Proccess*>* executionQueue,
    std::vector<Proccess*>* finishedProccesses,
    std::vector<Proccess*>* blockedProcesses,
    MemoryManager* memoryManager,
    unsigned time)
{
    #ifdef M_DEBUG
    assert(executionQueue     != nullptr &&
           finishedProccesses != nullptr &&
           blockedProcesses   != nullptr &&
           memoryManager      != nullptr);
    #endif

    if(executionQueue->empty()) return;

    // Current proccess has used all it's available quantum
    // OR the first proccess is about to start.
    if(m_currentProccessTimeLeft == 0)
    {
        // Execution is about to start
        if(executionQueue->front()->GetState() == ProccessState::READY)
        {
            MoveProccessWithEarliestDeadline(executionQueue);
            m_currentProccessTimeLeft = m_quantum - 1;
        }
        else if(executionQueue->front()->GetState() == ProccessState::DONE)
        {
            MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
            if(!executionQueue->empty()) 
            {
                m_currentProccessTimeLeft = m_quantum - 1;
                MoveProccessWithEarliestDeadline(executionQueue);
            }
        }
        else if(executionQueue->front()->GetState() == ProccessState::RUNNING)
        {
            executionQueue->front()->SetState(ProccessState::OVERHEAD);
            m_overheadTimeLeft = m_overhead - 1;
        }
        // Proccess state is being saved (overhead)
        else
        {
            if(m_overheadTimeLeft > 0)
            {
                m_overheadTimeLeft--;
            }
            else
            {
                m_overheadTimeLeft = 0;
                executionQueue->front()->SetState(ProccessState::READY);
                if(executionQueue->size() > 1)
                {
                    MoveProccessWithEarliestDeadline(executionQueue);
                }
                m_currentProccessTimeLeft = m_quantum - 1;
            }
        }
    }
    else 
    {
        if(executionQueue->front()->GetState() == ProccessState::DONE)
        {
            MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
            if(!executionQueue->empty())
            {
                MoveProccessWithEarliestDeadline(executionQueue);
                m_currentProccessTimeLeft = m_quantum - 1;
            }
            // There are no more proccesses in the execution queue
            else m_currentProccessTimeLeft = 0;
        }
        else
        {
            m_currentProccessTimeLeft--;
        }
    }

    if(!executionQueue->empty())
        executionQueue->front()->Run(time);
}

void Scheduler::MoveShortestJobToTheFront(std::vector<Proccess*>* executionQueue)
{
 // Pointer is invalid
    #ifdef M_DEBUG
    assert(executionQueue != nullptr);
    #endif

    // If there is only one or less active proccesses,
    // nothings needs to be done.
    if(executionQueue->size() > 1)
    {
        Proccess* shortestJob = executionQueue->front();
        std::vector<Proccess*>::iterator shortestJobIt = executionQueue->begin();
        unsigned shortestJobDuration = shortestJob->GetDuration();

        for(auto p = executionQueue->begin();
            p != executionQueue->end();
            p++)
        {
            if((*p)->GetDuration() < shortestJobDuration)
            {
                shortestJob = (*p);
                shortestJobIt = p;
                shortestJobDuration = (*p)->GetDuration();
            }
        }

        // If the shortest job is not at the front
        // of the execution queue, insert it there.
        if(shortestJob != executionQueue->front())
        {
            Proccess* currentFirstProccess = executionQueue->front();
            (*executionQueue)[0] = shortestJob;
            (*shortestJobIt) = currentFirstProccess;
        }        
    }
}

void Scheduler::MoveCurrentProccessToFinishedList(
        std::vector<Proccess*>* executionQueue,
        std::vector<Proccess*>* finishedProccesses)
{
    Proccess* currentProccess = executionQueue->front();
    finishedProccesses->push_back(currentProccess);
    executionQueue->erase(executionQueue->begin());
}

void Scheduler::MoveCurrentProccessToEndOfQueue(std::vector<Proccess*>* executionQueue)
{
    if(executionQueue == nullptr) return;
    if(executionQueue->size() < 2) return;

    Proccess* currentProccess = executionQueue->front();
    executionQueue->erase(executionQueue->begin());
    executionQueue->push_back(currentProccess);
}

void Scheduler::MoveProccessWithEarliestDeadline(std::vector<Proccess*>* executionQueue)
{
    // Pointer is invalid
    if(executionQueue == nullptr) return;

    // If there is only one or less active proccesses,
    // nothings needs to be done.
    if(executionQueue->size() > 1)
    {
        Proccess* earliestDeadlProcc = executionQueue->front();
        std::vector<Proccess*>::iterator earliestDeadlProccIt = executionQueue->begin();
        unsigned earliestDeadline = earliestDeadlProcc->GetDeadline();

        for(auto p = executionQueue->begin();
            p != executionQueue->end();
            p++)
        {
            if((*p)->GetDeadline() < earliestDeadline)
            {
                earliestDeadlProcc = (*p);
                earliestDeadlProccIt = p;
                earliestDeadline = (*p)->GetDeadline();
            }
            // If both proccesses have the same ID 
            // give priority to the one with lower id.
            // (This is done to standardize the order 
            // and make it predictable to the user,
            // because the order in the execution queue
            // can be disorganized).
            else if((*p)->GetDeadline() == earliestDeadline)
            {
                if((*p)->GetID() < earliestDeadlProcc->GetID())
                {
                    earliestDeadlProcc = (*p);
                    earliestDeadlProccIt = p;
                    earliestDeadline = (*p)->GetDeadline();
                } 
            }
        }

        // If the shortest job is not at the front
        // of the execution queue, insert it there.
        if(earliestDeadlProcc != executionQueue->front())
        {
            Proccess* currentFirstProccess = executionQueue->front();
            (*executionQueue)[0] = earliestDeadlProcc;
            (*earliestDeadlProccIt) = currentFirstProccess;
        }        
    }
}