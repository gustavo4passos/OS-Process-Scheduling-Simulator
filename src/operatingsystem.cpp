#include "operatingsystem.h"
#include "proccess.h"
#include "scheduler.h"

#include <iostream>

OperatingSystem::OperatingSystem(
    int quantum,
    int overhead,
    SchedulingAlgorithm schedulingAlgorithm)
:   m_quantum(quantum),
    m_overhead(overhead),
    m_time(0)
{
    m_scheduler = new Scheduler(quantum, overhead, schedulingAlgorithm);
}

float OperatingSystem::GetAverageTurnaround() const
{
    // No proccess has ever been finished
    if(m_finishedProccesses.size() == 0) return 0;

    int totalExecutionTime = 0;
    int numberOfFinishedProccessses = m_finishedProccesses.size();

    for(auto proccess = m_finishedProccesses.begin();
        proccess != m_finishedProccesses.end();
        proccess++)
    {
        totalExecutionTime += (*proccess)->GetTurnAround();
    }

    return (float)totalExecutionTime / numberOfFinishedProccessses;
}

void OperatingSystem::AddProccess(Proccess* proccess)
{
    if(proccess != nullptr) 
    {
        m_proccesses.push_back(proccess);
        m_numberOfProccesses++;
    }
}

bool OperatingSystem::NextStep()
{
    // If there are no proccesses in execution
    // and no proccesses to arrive
    // there is nothing to be done.
    if(m_executionQueue.size() == 0 && m_proccesses.size() == 0) return false;

    std::cout << "Moment " << m_time << std::endl;;
    UpdateExecutionQueue();

    // Advances time
    m_time++;    

    // There are proccesses active at the moment
    // Note(Gustavo): This verification is needed
    // because it's possible that the execution queue
    // is empty (no active proccesses at the moment),
    // but proccesses will arrive later.
    if(m_executionQueue.size() > 0)
    {
        m_scheduler->Run(&m_executionQueue, &m_finishedProccesses, m_time);
        if(m_executionQueue.size() > 0) m_executionQueue.front()->Run(m_time);
    }     

    return true;
}

void OperatingSystem::Reset()
{
    m_scheduler->Reset();

    // Put current running proccesses 
    // back in waiting list.
    for(auto p = m_executionQueue.begin();
        p != m_executionQueue.end();)
    {
        m_proccesses.push_back(*p);
        m_executionQueue.erase(p);
    }

    // Put finished proccesses back in
    // waiting list.
    for(auto p = m_finishedProccesses.begin();
        p != m_finishedProccesses.end())
    {
        m_proccesses.push_back(*p);
        m_finishedProccesses.erase(p);
    }

    // Reset all proccesses
    for(auto p = m_proccesses.begin();
        p != m_proccesses.end();
        p++)
    {
        (*p)->Reset();
    }
}

void OperatingSystem::UpdateExecutionQueue()
{
    for(auto proccess = m_proccesses.begin(); 
        proccess != m_proccesses.end();)
    {
        if((*proccess)->GetArrivalTime() == m_time)
        {
            m_executionQueue.push_back(*proccess);
            std::cout << "Proccess " << (*proccess)->GetID() << " arrived." << std::endl;
            m_proccesses.erase(proccess);
        }
        else 
        {
            proccess++;
        }
    }
}