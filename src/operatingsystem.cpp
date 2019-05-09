#include "operatingsystem.h"
#include "proccess.h"
#include "scheduler.h"

#include <iostream>

OperatingSystem::OperatingSystem(
    int quantum,
    int overload,
    SchedulingAlgorithm schedulingAlgorithm)
:   m_quantum(quantum),
    m_overload(overload),
    m_time(0),
    m_currentProccessTimeLeft(0)
{
    m_scheduler = new Scheduler(quantum, overload, schedulingAlgorithm);

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
    if(proccess != nullptr) m_proccesses.push_back(proccess);
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

    // There are proccess active at the moment
    // Note(Gustavo): This verification is needed
    // because it's possible that the execution queue
    // is empty (no active proccesses at the moment),
    // but proccesses will arrive later.
    if(m_executionQueue.size() > 0)
    {
        // The current running procces has finished, so
        // it must be removed from the execution queue (no overload)
        if(m_executionQueue.front()->GetState() == ProccessState::DONE)
        {
            std::cout << "Proccess " << m_executionQueue.front()->GetID() << " is done." << std::endl;
            m_finishedProccesses.push_back(m_executionQueue.front());
            m_executionQueue.erase(m_executionQueue.begin());
            m_scheduler->Run(&m_executionQueue);
            m_currentProccessTimeLeft = m_quantum;

            if(m_executionQueue.size() > 0)
            {
                m_executionQueue.front()->SetState(ProccessState::RUNNING);
            }
        }

        // The current running procces has used all
        // its available quantum
        if(m_currentProccessTimeLeft == 0)
        {
            Proccess* currentRunningProccess = m_executionQueue.front();
            currentRunningProccess->SetState(ProccessState::IDLE);

            m_scheduler->Run(&m_executionQueue);
            
            // The current proccess has changed,
            // so the OS needs to store the previous
            // proccess state (overload)
            if(currentRunningProccess->GetID() != m_executionQueue.front()->GetID())
            {
                currentRunningProccess->SetState(ProccessState::OVERLOAD);   
                m_currentProccessOverloadLeft = m_overload;
            }
            else 
            {
                currentRunningProccess->SetState(ProccessState::RUNNING);
            }
            m_currentProccessTimeLeft = m_quantum;
        }

        if(m_executionQueue.front()->GetState() == ProccessState::OVERLOAD)
        {
            if(m_currentProccessOverloadLeft > 0) m_currentProccessOverloadLeft--;
            else m_executionQueue.front()->SetState(ProccessState::RUNNING);
        }

        if(m_executionQueue.front()->GetState() == ProccessState::RUNNING)
        {
            m_executionQueue.front()->Run(m_time);
            m_currentProccessTimeLeft--;
        }
    }     

    return true;
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