#ifndef PROCCESS_H
#define PROCCESS_H

enum class ProccessState
{
  IDLE,
  RUNNING,
  OVERHEAD,
  DONE
};

class Proccess
{
public:
    Proccess(
        unsigned ID,
        unsigned arrivalTime,
        unsigned duration,
        unsigned deadlinem
    );

    inline unsigned GetID()          const { return m_ID;          }
    inline unsigned GetArrivalTime() const { return m_arrivalTime; }
    inline unsigned GetDuration()    const { return m_duration;    }
    inline unsigned GetTimeLeft()    const { return m_timeLeft;    }
    inline unsigned GetDeadline()    const { return m_deadline;    }
    // Returns when the proccess finished.
    // If the proccess has not yet finished,
    // it returns 0.
    inline unsigned GetFinishTime()  const { return m_finishTime;  }
    inline bool IsFinished()         const { return m_finished;    }

    inline int GetTurnAround()  const { return m_finishTime - m_arrivalTime; }
    inline ProccessState GetState() const { return m_state; }
    inline void SetState(ProccessState state) { m_state = state; }
    inline void SetFinishTime(int time) { m_finishTime = time; }

    void Run(int currentTime);
    // Reset procccess to its initial state
    void Reset();


    // Sort proccess based on arrival time
    bool operator< (const Proccess& p) { return m_arrivalTime < p.m_arrivalTime; }

private:
    unsigned m_ID;
    unsigned m_arrivalTime;
    unsigned m_duration;
    unsigned m_timeLeft;
    unsigned m_deadline;
    ProccessState m_state;
    bool m_finished;
    unsigned m_finishTime;
};

#endif
