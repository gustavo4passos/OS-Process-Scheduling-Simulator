#include "scheduler.h"
#include "proccess.h"

void Scheduler::Run(std::vector<Proccess*>* executionQueue)
{
    switch(m_algorithm)
    {
        case SchedulingAlgorithm::FIFO:
            RunFIFO(executionQueue);
            break;
        
        case SchedulingAlgorithm::SJF:
            RunSJF(executionQueue);
            break;
        
        case SchedulingAlgorithm::ROUND_ROBIN:
            RunRoundRobin(executionQueue);
            break;

        case SchedulingAlgorithm::EDF:
            RunEDF(executionQueue);
            break;
        
        default:
            break;
    }
}

void Scheduler::RunFIFO(std::vector<Proccess*>* executionQueue)
{
    return;
}

void Scheduler::RunSJF(std::vector<Proccess*>* executionQueue) 
{ 
    // Pointer is invalid
    if(executionQueue == nullptr) return;

    if(executionQueue->size() > 0)
    {
        Proccess* shortestJob = executionQueue->front();
        int shortestJobDuration = shortestJob->GetDuration();

        for(auto p = executionQueue->begin();
            p != executionQueue->end();
            p++)
        {
            if((*p)->GetDuration() < shortestJobDuration)
            {
                shortestJobDuration = (*p)->GetDuration();
                shortestJob = (*p);
            }
        }

        
    }
}

void Scheduler::RunRoundRobin(std::vector<Proccess*>* executionQueue) 
{ 

}

void Scheduler::RunEDF(std::vector<Proccess*>* executionQueue) 
{ 
    
}