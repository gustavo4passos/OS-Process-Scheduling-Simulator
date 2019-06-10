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

void Scheduler::DoIO(
    std::vector<Proccess*>* executionQueue, 
    std::vector<Proccess*>* blockedProccesses,
    MemoryManager* memoryManager)
{
    if(blockedProccesses->empty()) return;

    Proccess* proccess = blockedProccesses->front();
    proccess->SetState(ProccessState::IO);
    memoryManager->LoadAbsentPagesToMemory(proccess->GetPages());

    if(memoryManager->AreAllPagesInMemory(proccess->GetPages()))
    {
        // If proccess is done loading, remove if from the blocked list
        blockedProccesses->erase(blockedProccesses->begin());

        if(m_algorithm == SchedulingAlgorithm::FIFO)
        {
            // And insert it at the begining of the execution queue
            executionQueue->insert(executionQueue->begin(), proccess);
        }
        else
        {
            executionQueue->push_back(proccess);
        }
    }
}

void Scheduler::BlockProcess(
    Proccess* proccess,
    std::vector<Proccess*>* blockedProccesses)
{
    #ifdef M_DEBUG
    assert(proccess != nullptr);
    #endif

    proccess->SetState(ProccessState::BLOCKED);
    blockedProccesses->push_back(proccess);
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

    UnblockProccessesInExecutionQueue(executionQueue);

    if(executionQueue->empty() && blockedProcesses->empty()) return;

    // Current process has finished running
    if(executionQueue->front()->GetState() == ProccessState::DONE)
    {
        // Remove proccess pages from RAM
        memoryManager->DeletePages(executionQueue->front()->GetPages());
        MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
    }

    if(!executionQueue->empty() && blockedProcesses->empty())
    {
        Proccess* currentProccess = executionQueue->front();

        // Check if current proccess has all pages in memory
        if(!memoryManager->AreAllPagesInMemory(currentProccess->GetPages()))
        {
            BlockProcess(currentProccess, blockedProcesses);
            executionQueue->erase(executionQueue->begin());
        }
        else
        {
            if(currentProccess->GetState() == ProccessState::BLOCKED)
            {
                currentProccess->SetState(ProccessState::READY);
            }
            currentProccess->Run(time);
        }
    }
    if(!blockedProcesses->empty())
    {
        DoIO(executionQueue, blockedProcesses, memoryManager); 
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
        memoryManager->DeletePages(executionQueue->front()->GetPages());
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
        if(currentProccess->GetState() == ProccessState::IO)
        {
            currentProccess->SetState(ProccessState::RUNNING);
        }
        executionQueue->front()->Run(time);
    }
    else
    {
        currentProccess->SetState(ProccessState::IO);
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
    if(executionQueue->empty() && blockedProcesses->empty()) return;

    UnblockProccessesInExecutionQueue(executionQueue);

    // Current proccess has used all it's available quantum
    // OR the first proccess is about to start.
    if(m_currentProccessTimeLeft == 0)
    {
        // Execution is about to start
        if(executionQueue->front()->GetState() == ProccessState::READY)
        {
            FindNextProccessWithPagesInMemory(
                executionQueue, 
                blockedProcesses, 
                memoryManager);
            
            if(!executionQueue->empty()) m_currentProccessTimeLeft = m_quantum - 1;
        }
        // Current proccess has finished executing
        else if(executionQueue->front()->GetState() == ProccessState::DONE)
        {
            // Clear the pages of the current process from memory
            memoryManager->DeletePages(executionQueue->front()->GetPages());
            MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
            
            if(!executionQueue->empty()) 
            {
                FindNextProccessWithPagesInMemory(
                executionQueue, 
                blockedProcesses, 
                memoryManager);

                if(!executionQueue->empty())
                {
                    // Restarts quantum
                    m_currentProccessTimeLeft = m_quantum - 1;
                }
            }
        }
        // The process is still running, but has already used all it's available
        // quantum, so context change will start (overhead)
        else if(executionQueue->front()->GetState() == ProccessState::RUNNING)
        {
            executionQueue->front()->SetState(ProccessState::OVERHEAD);
            m_overheadTimeLeft = m_overhead - 1;
        }
        // Proccess state is being saved (context change)
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

                // There are other processes to be executed
                if(executionQueue->size() > 1)
                {
                    // Unprotect process pages before next process runs
                    memoryManager->RemoveProtectionFromPages(
                        executionQueue->front()->GetPages());
                    MoveCurrentProccessToEndOfQueue(executionQueue);

                    FindNextProccessWithPagesInMemory(
                        executionQueue, 
                        blockedProcesses, 
                        memoryManager);
                }

                if(!executionQueue->empty())
                {
                    // Reset quantum
                    m_currentProccessTimeLeft = m_quantum - 1;
                }
            }
        }
    }
    else // The current running process still have quantum left to use
    {
        // The processed finished running before using it's available quantum
        if(executionQueue->front()->GetState() == ProccessState::DONE)
        {
            // Clear proccess pages from memory, as it is done running
            memoryManager->DeletePages(executionQueue->front()->GetPages());
            MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);

            // If there are still proccesses to run that are not blocked
            if(!executionQueue->empty()) 
            {
                FindNextProccessWithPagesInMemory(
                    executionQueue, 
                    blockedProcesses, 
                    memoryManager);

                // If an available proccess was found
                if(!executionQueue->empty())
                {
                    m_currentProccessTimeLeft = m_quantum - 1;
                }
            }
            else m_currentProccessTimeLeft = 0; 
        }
        else
        {
            m_currentProccessTimeLeft--;
        }  
    }

    if(executionQueue->size() > 0) 
    {
        executionQueue->front()->Run(time); 
        // Mark the proccess pages as referenced
        memoryManager->ReferencePages(executionQueue->front()->GetPages());
    }
    DoIO(executionQueue, blockedProcesses, memoryManager);
}

void Scheduler::RunEDF(
    std::vector<Proccess*>* executionQueue,
    std::vector<Proccess*>* finishedProccesses,
    std::vector<Proccess*>* blockedProccesses,
    MemoryManager* memoryManager,
    unsigned time)
{
    #ifdef M_DEBUG
    assert(executionQueue     != nullptr &&
           finishedProccesses != nullptr &&
           blockedProccesses   != nullptr &&
           memoryManager      != nullptr);
    #endif

    UnblockProccessesInExecutionQueue(executionQueue);

    if(executionQueue->empty() && blockedProccesses->empty()) return;

    // Current proccess has used all it's available quantum
    // OR the first proccess is about to start.
    if(m_currentProccessTimeLeft == 0)
    {
        // Execution is about to start
        if(executionQueue->front()->GetState() == ProccessState::READY)
        {
            FindNextProccessWithPagesInMemory(
                executionQueue,
                blockedProccesses,
                memoryManager);
            // If there are any processes available to run, restart quantum
            if(!executionQueue->empty())
            {
                m_currentProccessTimeLeft = m_quantum - 1;
            }
        } 
        // Currrent proccess has finished executing
        else if(executionQueue->front()->GetState() == ProccessState::DONE)
        {
            memoryManager->DeletePages(executionQueue->front()->GetPages());
            MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);

            FindNextProccessWithPagesInMemory(
                executionQueue,
                blockedProccesses,
                memoryManager);

            // If there are any processes available to run, restart quantum
            if(!executionQueue->empty())
            {
                m_currentProccessTimeLeft = m_quantum - 1;
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
                // Context saving has finished, change current proccess
                // state from running to ready
                m_overheadTimeLeft = 0;
                executionQueue->front()->SetState(ProccessState::READY);

                // If there are more proccesses in the execution
                // list, unprotect the pages from the current
                // proccess and find the next proccess to run
                if(executionQueue->size() > 1)
                {
                    memoryManager->RemoveProtectionFromPages(
                        executionQueue->front()->GetPages());
                    FindNextProccessWithPagesInMemory(
                        executionQueue,
                        blockedProccesses,
                        memoryManager);
                }
                // If there are any processes available to run, restart quantum
                if(!executionQueue->empty())
                {
                    m_currentProccessTimeLeft = m_quantum - 1;
                }
            }
        }
    }
    else 
    {
        if(executionQueue->front()->GetState() == ProccessState::DONE)
        {
            memoryManager->DeletePages(executionQueue->front()->GetPages());
            MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);

            FindNextProccessWithPagesInMemory(
                executionQueue,
                blockedProccesses,
                memoryManager);

            // If there are any processes available to run, restart quantum
            if(!executionQueue->empty())
            {
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
    {
        executionQueue->front()->Run(time);
        // Mark the proccess pages as referenced
        memoryManager->ReferencePages(executionQueue->front()->GetPages());
    }
    DoIO(executionQueue, blockedProccesses, memoryManager);
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

void Scheduler::FindNextProccessWithPagesInMemory(
    std::vector<Proccess*>* executionQueue,
        std::vector<Proccess*>* blockedProccesses,
        MemoryManager* memoryManager)
{
    while(!executionQueue->empty())
    {
        Proccess* proccess;
        // Choose next possible proccess using the current algorithm
        if(m_algorithm == SchedulingAlgorithm::ROUND_ROBIN)
        {
            proccess = executionQueue->front();
        }
        else if(m_algorithm == SchedulingAlgorithm::EDF)
        {
            MoveProccessWithEarliestDeadline(executionQueue);
            proccess = executionQueue->front();
        }
        else assert(false);

        // Proccess can run
        if(memoryManager->AreAllPagesInMemory(proccess->GetPages()))
        {
            // Protect current proccess pages
            memoryManager->ProtectPages(executionQueue->front()->GetPages());
            break;
        }
        else
        {
            // Not all pages from current proccesses are in memory,
            // Block it.
            BlockProcess(proccess, blockedProccesses);
            executionQueue->erase(executionQueue->begin());
        } 
    }
}

void Scheduler::
    UnblockProccessesInExecutionQueue(std::vector<Proccess*>* executionQueue)
{
    for(auto& proccess : *executionQueue)
    {
        if(proccess->GetState() == ProccessState::BLOCKED ||
           proccess->GetState() == ProccessState::IO)
        {
            proccess->SetState(ProccessState::READY);
        }
    }
}