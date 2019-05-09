#ifndef PROCCESS_H
#define PROCCESS_H

enum class ProccessState
{
  IDLE,
  RUNNING,
  OVERLOAD,
  DONE
};

class Proccess
{
public:
    Proccess(
        int ID,
        int arrivalTime,
        int duration,
        int deadline,
        int priority
    );

    inline int GetID()          const { return m_ID;          }
    inline int GetArrivalTime() const { return m_arrivalTime; }
    inline int GetDuration()    const { return m_duration;    }
    inline int GetTimeLeft()    const { return m_timeLeft;    }
    inline int GetDeadline()    const { return m_deadline;    }
    inline int GetPriority()    const { return m_priority;    }
    inline int GetFinishTime()  const { return m_finishTime;  }
    inline bool IsFinished()    const { return m_finished;    }

    inline int GetTurnAround()  const { return m_finishTime - m_arrivalTime; }
    inline ProccessState GetState() const { return m_state; }
    inline void SetState(ProccessState state) { m_state = state; }
    inline void SetFinishTime(int time) { m_finishTime = time; }

    void Run(int currentTime);


    // Sort proccess based on arrival time
    bool operator< (const Proccess& p) { return m_arrivalTime < p.m_arrivalTime; }

private:
    int m_ID;
    int m_arrivalTime;
    int m_duration;
    int m_timeLeft;
    int m_deadline;
    int m_priority;
    ProccessState m_state;
    bool m_finished;
    int m_finishTime;
};

#endif
