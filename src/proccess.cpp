#include "proccess.h"
#include <iostream>

Proccess::Proccess(
    unsigned ID, 
    unsigned arrivalTime,
    unsigned duration,
    unsigned deadline,
    unsigned priority)
:   m_ID(ID),
    m_arrivalTime(arrivalTime),
    m_duration(duration),
    m_timeLeft(duration),
    m_deadline(deadline),
    m_priority(priority),
    m_state(ProccessState::IDLE),
    m_finished(false),
    m_finishTime(0)
{ }

void Proccess::Run(int currentTime)
{
    // Proccess is done, there is nothing to be done
    if(m_state == ProccessState::DONE) return;

    // If proccess is currently idle, change its state
    if(m_state == ProccessState::IDLE)
    {
        m_state = ProccessState::RUNNING;
    }
    
    if(m_state == ProccessState::OVERHEAD)
    {
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
    
}

void Proccess::Reset()
{
    m_timeLeft = m_duration;
    m_state = ProccessState::IDLE;
    m_finished = false;
    m_finishTime = 0;
}