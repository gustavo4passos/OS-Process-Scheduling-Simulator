#include "scheduler.h"
#include "proccess.h"

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
    int currentTime)
{
    switch(m_algorithm)
    {
        case SchedulingAlgorithm::FIFO:
            RunFIFO(executionQueue, finishedProccesses, currentTime);
            break;
        
        case SchedulingAlgorithm::SJF:
            RunSJF(executionQueue, finishedProccesses, currentTime);
            break;
        
        case SchedulingAlgorithm::ROUND_ROBIN:
            RunRoundRobin(executionQueue, finishedProccesses, currentTime);
            break;

        case SchedulingAlgorithm::EDF:
            RunEDF(executionQueue, finishedProccesses, currentTime);
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
        (*p)->SetState(ProccessState::IDLE);
    }
}

void Scheduler::RunFIFO(
    std::vector<Proccess*>* executionQueue,
    std::vector<Proccess*>* finishedProccesses, 
    int currentTime)
{
    if(executionQueue == nullptr || finishedProccesses == nullptr) return;
    if(executionQueue->empty()) return;

    if(executionQueue->front()->GetState() == ProccessState::DONE)
    {
        MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
    }
}

void Scheduler::RunSJF(
    std::vector<Proccess*>* executionQueue,
    std::vector<Proccess*>* finishedProccesses, 
    int currentTime) 
{ 
    // Check if pointers are valid
    if(executionQueue == nullptr || finishedProccesses == nullptr) return;
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
    if(executionQueue->front()->GetState() == ProccessState::IDLE)
    {
        MoveShortestJobToTheFront(executionQueue);
    }
}

void Scheduler::RunRoundRobin(
    std::vector<Proccess*>* executionQueue,
    std::vector<Proccess*>* finishedProccesses, 
    int currentTime) 
{ 
    if(executionQueue == nullptr || finishedProccesses == nullptr) return;
    if(executionQueue->empty()) return;

    // Current proccess has used all it's available quantum
    // OR the first proccess is about to start.
    if(m_currentProccessTimeLeft == 0)
    {
        // Execution is about to start
        if(executionQueue->front()->GetState() == ProccessState::IDLE)
        {
            m_currentProccessTimeLeft = m_quantum - 1;
            return;
        }
        else if(executionQueue->front()->GetState() == ProccessState::DONE)
        {
            MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
            if(!executionQueue->empty()) m_currentProccessTimeLeft = m_quantum - 1;
            return;
        }
        else if(executionQueue->front()->GetState() == ProccessState::RUNNING)
        {
            executionQueue->front()->SetState(ProccessState::OVERHEAD);
            m_overheadTimeLeft = m_overhead - 1;
            return;
        }
        // Proccess state is being saved (overhead)
        else
        {
            if(m_overheadTimeLeft > 0)
            {
                m_overheadTimeLeft--;
                return;
            }
            else
            {
                m_overheadTimeLeft = 0;
                executionQueue->front()->SetState(ProccessState::IDLE);
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
            return;
        }
        else
        {
            m_currentProccessTimeLeft--;
            return;
        }
        
    }
}

void Scheduler::RunEDF(
    std::vector<Proccess*>* executionQueue,
    std::vector<Proccess*>* finishedProccesses, 
    int currentTime) 
{ 
    if(executionQueue == nullptr || finishedProccesses == nullptr) return;
    if(executionQueue->empty()) return;

    // Current proccess has used all it's available quantum
    // OR the first proccess is about to start.
    if(m_currentProccessTimeLeft == 0)
    {
        // Execution is about to start
        if(executionQueue->front()->GetState() == ProccessState::IDLE)
        {
            MoveProccessWithEarliestDeadline(executionQueue);
            m_currentProccessTimeLeft = m_quantum - 1;
            return;
        }
        else if(executionQueue->front()->GetState() == ProccessState::DONE)
        {
            MoveCurrentProccessToFinishedList(executionQueue, finishedProccesses);
            if(!executionQueue->empty()) 
            {
                m_currentProccessTimeLeft = m_quantum - 1;
                MoveProccessWithEarliestDeadline(executionQueue);
            }
            return;
        }
        else if(executionQueue->front()->GetState() == ProccessState::RUNNING)
        {
            executionQueue->front()->SetState(ProccessState::OVERHEAD);
            m_overheadTimeLeft = m_overhead - 1;
            return;
        }
        // Proccess state is being saved (overhead)
        else
        {
            if(m_overheadTimeLeft > 0)
            {
                m_overheadTimeLeft--;
                return;
            }
            else
            {
                m_overheadTimeLeft = 0;
                executionQueue->front()->SetState(ProccessState::IDLE);
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
            return;
        }
        else
        {
            m_currentProccessTimeLeft--;
            return;
        }
        
    }
}

void Scheduler::MoveShortestJobToTheFront(std::vector<Proccess*>* executionQueue)
{
 // Pointer is invalid
    if(executionQueue == nullptr) return;

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