#include "proccess.h"
#include <iostream>

Proccess::Proccess(
    int ID, 
    int arrivalTime,
    int duration,
    int deadline,
    int priority)
:   m_ID(ID),
    m_arrivalTime(arrivalTime),
    m_duration(duration),
    m_timeLeft(duration),
    m_deadline(deadline),
    m_priority(priority),
    m_state(ProccessState::IDLE)
{ }

void Proccess::Run(int currentTime)
{
    if(m_state == ProccessState::DONE) return;

    if(m_state == ProccessState::IDLE)
    {
        m_state = ProccessState::RUNNING;
    }
    if(m_state == ProccessState::OVERHEAD)
    {
        std::cout << "Overhead of proccess " << m_ID << std::endl;
        return;
    }

    m_timeLeft--;

    // Proccess is done.
    // Sets the state as done and stores the time 
    // it finished.
    if(m_timeLeft == 0)
    {
        m_state = ProccessState::DONE;
        m_finishTime = currentTime;
    }
    
    std::cout << "Running proccess " << m_ID << " time left " << m_timeLeft << std::endl;
}