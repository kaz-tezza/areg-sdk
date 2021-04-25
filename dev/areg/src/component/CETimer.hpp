#ifndef AREG_COMPONENT_CETIMER_HPP
#define AREG_COMPONENT_CETIMER_HPP
/************************************************************************
 * \file        areg/src/component/CETimer.hpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Timer class.
 *              Use to fire timer.
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/src/base/GEGlobal.h"

#include "areg/src/base/CEString.hpp"
#include "areg/src/base/ESynchObjects.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class IETimerConsumer;
class CEDispatcherThread;

//////////////////////////////////////////////////////////////////////////
// CETimer class declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief       The timer is used to fire event in a certain period of time.
 *              When timer is expired, it will trigger Timer Event and
 *              forward to specified Timer Consumer object to process.
 * 
 * \details     The Timer object is used for periodic event.
 *              Create Timer and pass consumer object, which will process
 *              the Timer Event. The Consumer object cannot be changed.
 *              The Timer Event will be processed in the thread,
 *              which was requesting to start timer.
 *              The Timer Events are processed only in Dispatcher Threads.
 *              If Timer is requested to start in non-dispatcher thread,
 *              the starting will be ignored and no timer event will be processed.
 *
 **/
class AREG_API CETimer
{
/************************************************************************/
// Friend classes to access protected members.
/************************************************************************/

    friend class CETimerInfo;
    friend class CETimerEvent;

//////////////////////////////////////////////////////////////////////////
// Predefined constants and types
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   CETimer::CONTINUOUSLY
     *          This value is used to set continues Timer, which will not
     *          stop, until it is not requested to be stopped manually.
     **/
    static const unsigned int   CONTINUOUSLY          /*= static_cast<unsigned int>(-1)*/;    /*0xFFFFFFFF*/

    /**
     * \brief   CETimer::DEFAULT_MAXIMUM_QUEUE
     *          Default number of maximum queued number of timer events in dispatcher thread.
     **/
    static const int            DEFAULT_MAXIMUM_QUEUE /*= static_cast<int>(5)*/;            /*0x00000005*/

    /**
     * \brief   CETimer::IGNORE_TIMER_QUEUE
     *          Defined to ignore number of maximum queued timer events in dispatcher thread.
     **/
    static const int            IGNORE_TIMER_QUEUE    /*= static_cast<int>(0)*/;            /*0x00000000*/

    /**
     * \brief   1 millisecond
     **/
    static const unsigned int   TIMEOUT_1_MS            /*= 1*/;

    /**
     * \brief   100 milliseconds
     **/
    static const unsigned int   TIMEOUT_100_MS          /*= (TIMEOUT_1_MS * 100)*/;

    /**
     * \brief   500 milliseconds
     **/
    static const unsigned int   TIMEOUT_500_MS          /*= (TIMEOUT_100_MS * 5)*/;

    /**
     * \brief   1 second in milliseconds
     **/
    static const unsigned int   TIMEOUT_1_SEC           /*= (TIMEOUT_1_MS * 1000)*/;

    /**
     * \brief   1 minute in milliseconds
     **/
    static const unsigned int   TIMEOUT_1_MIN           /*= (TIMEOUT_1_SEC * 60)*/;

    /**
     * \brief   Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days
     **/
    static unsigned int GetTickCount( void );

//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   The constructor sets Timer object consumer and optional the name.
     *          If timer name is not NULL and to provide uniqueness of names in the system, 
     *          the system will use passed name as a prefix.
     * \param   timerConsumer   The Timer Consumer object.
     * \param   timerName       The name of Timer. If this is not NULL, the system will 
     *                          generate unique name using passed name as a prefix.
     *                          The timer objects must have unique names and should not
     *                          contain back slashes.
     * \param   maxQueued       The maximum number of timer events queued in dispatcher thread.
     *                          This parameter controls number of fired timer events in dispatcher thread.
     *                          If currently queued timer events are more or equal than this number,
     *                          the timer temporary will be stopped and fired again, when number timer
     *                          timer events are less than allowed maximum number of timer events in queue.
     *                          This parameter makes sense only for continues timer. For timers,
     *                          fired once or less than maxQueued timer, this parameter will be ignored
     *                          and play no role.
     **/
    CETimer( IETimerConsumer & timerConsumer, const char* timerName = NULL, int maxQueued = CETimer::DEFAULT_MAXIMUM_QUEUE );
    /**
     * \brief   Destructor
     **/
    ~CETimer( void );

//////////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Call to start timer. The system will send and the
     *          the Timer consumer object will receive timer event to process
     *          every time when specified timeout in milliseconds expires.
     *          The events will be send specified event count times and will
     *          be dispatched in the current thread context. If eventCount is,
     *          the events will be sent continuously until timer is not stopped.
     *          If the timer was already started and requested new start,
     *          the ongoing timer will be stopped and canceled, and reactivated
     *          with new parameters.
     * \param   timeoutInMs Timeout to fire timer event
     * \param   eventCount  The number of events to fire. 
     *                      If this value is zero, no time event will be fired.
     *                      If this value is CONTINUOUSLY, the event fill be triggered
     *                      until it will not be stopped manually.
     *                      Otherwise, timer will be triggered until event count 
     *                      reached specified number.
     * \return  Returns true if Timer was successfully started.
     **/
    bool StartTimer(unsigned int timeoutInMs, unsigned int eventCount = CETimer::CONTINUOUSLY);

    /**
     * \brief   Call to start timer. The system will send and the
     *          the Timer consumer object will receive timer event to process
     *          every time when specified timeout in milliseconds expires.
     *          The events will be send specified event count times and will
     *          be dispatched in the specified thread context. If eventCount is,
     *          the events will be sent continuously until timer is not stopped.
     *          If the timer was already started and requested new start,
     *          the ongoing timer will be stopped and canceled, and reactivated
     *          with new parameters.
     * \param   timeoutInMs Timeout to fire timer event.
     * \param   whichThread The dispatcher thread, which should process the fired
     *                      timer event. Should be valid dispatcher thread.
     * \param   eventCount  The number of events to fire. 
     *                      If this value is zero, no time event will be fired.
     *                      If this value is CONTINUOUSLY, the event fill be triggered
     *                      until it will not be stopped manually.
     *                      Otherwise, timer will be triggered until event count is not
     *                      reaching specified number.
     * \return  Returns true if Timer was successfully started.
     **/
    bool StartTimer(unsigned int timeoutInMs, CEDispatcherThread & whichThread, unsigned int eventCount = CETimer::CONTINUOUSLY);

    /**
     * \brief   Call to stop previously started timer.
     **/
    void StopTimer( void );

//////////////////////////////////////////////////////////////////////////
// Attributes
//////////////////////////////////////////////////////////////////////////
public:

    /**
     * \brief   Returns Timer Consumer object.
     **/
    inline IETimerConsumer & GetTimerConsumer( void ) const;

    /**
     * \brief   Returns unique name of timer. 
     *          This name will not match the name specified in constructor.
     *          To provide uniqueness, new name will be generated using passed
     *          name in constructor as a prefix.
     *          This function will return empty string, if no name was set in constructor.
     **/
    inline const CEString & GetTimerName( void ) const;

    /**
     * \brief   Returns the timeout value to fire time.
     **/
    inline unsigned int GetFireTime( void ) const;

    /**
     * \brief   Returns the amount of events, which timer still needs to send
     *          This function returns zero, if timer is stopped (automatically or manually),
     *          and returns CETimer::CONTINUOUSLY for continues events.
     **/
    inline unsigned int GetEventCount( void ) const;

    /**
     * \brief   Returns the value of next fire time in milliseconds, 
     *          since the since the system was started up to 49,7 days
     **/
    inline unsigned int GetNextFireTime( void ) const;

    /**
     * \brief   Returns the timeout value of the timer. If timer is stopped, this value is zero.
     **/
    inline unsigned int GetTimeoutValue( void ) const;

    /**
     * \brief   Returns true if timer is active.
     *          The timer is inactive, if event count is zero.
     **/
    inline bool IsActive( void ) const;

    /**
     * \brief   Returns true if timer is stopped. The timer is stopped if timeout value is zero.
     **/
    inline bool IsStopped( void ) const;

    /**
     * \brief   Return the remaining time to fire timer in milliseconds.
     *          The negative value indicates that either timer is inactive
     *          or it is fired.
     **/
    inline int GetTimePending( void ) const;

    /**
     * \brief   Returns true if timer has name.
     **/
    inline bool HasName( void ) const;

    /**
     * \brief   Returns the value of last fired time.
     *          The information is in Coordinated Universal Time (UTC) format.
     *          The returns value is in Contains 64-bit value representing the number 
     *          of 100-nanosecond (means in 100/1000000 milliseconds) intervals since January 1, 1601 (UTC).
     *          The value is not affected by changes in time zone or daylight saving time.
     **/
    inline const uint64_t & GetFiredTime( void ) const;

//////////////////////////////////////////////////////////////////////////
// Operations. Used by Timer Manager.
//////////////////////////////////////////////////////////////////////////
protected:

    /**
     * \brief   Called by Timer Manager before starting timer.
     *          If returns true, if the timer will start. 
     *          Otherwise it will be ignored.
     * \return  Returns true, if timer is active and started. 
     *          Otherwise, returns false.
     **/
    bool TimerIsStarting( void );

    /**
     * \brief   Called by timer manager when timer is expired.
     *          If function returns false (reached event count limitation),
     *          the timer will be stopped.
     *          The passed parameters are low and high 32-bits of
     *          64-bit time in Coordinated Universal Time (UTC) format.
     *          The value will be saved as a last fired time.
     * \param   highValue   Th high 32-bit value to set.
     * \param   lowValue    The low 32-bit value to set.
     * \return  Returns true, if timer is still active, i.e. the Event Count is not zero.
     *          Otherwise, if timer should be stopped, it returns false.
     **/
    bool TimerIsExpired(unsigned int highValue, unsigned int lowValue);

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   Timer consumer object
     **/
    IETimerConsumer &  mConsumer;
    /**
     * \brief   Timer name. If not empty, it is unique name
     **/
    CEString            mName;
    /**
     * \brief   Timeout to fire timer.
     **/
    unsigned int        mTimeoutInMs;
    /**
     * \brief   The amount of events to fire
     **/
    unsigned int        mEventsCount;
    /**
     * \brief   Next event to fire relative since the system was started, up to 49.7 days
     **/
    unsigned int        mNextFire;
    /**
     * \brief   The last fired time in Coordinated Universal Time (UTC) format.
     **/
    uint64_t            mLastFired;

private:
    /**
     * \brief   Number of timer events, currently queued dispatcher.
     **/
    int                 mCurrentQueued;
    /**
     * \brief   Maximum number of events, which should be queued in dispatcher.
     *          If currently queued events are more or equal than the maximum number
     *          of events currently queued in dispatcher thread, 
     **/
    const int           mMaxQueued;
    /**
     * \brief   The Dispatcher thread where currently the timer is dispatched.
     **/
    CEDispatcherThread *mDispatchThread;
    /**
     * \brief   Flag, indicating whether the timer is already started by timer manager or not.
     *          This flag is true, only when StartTimer of timer manager is called. 
     *          Otherwise it is false.
     *          The timer can be active, but stopped. This might happen when timer event
     *          reached its maximum queue number in the dispatcher thread (mMaxQueued).
     *          In this case the timer might be stopped on timer manager side, 
     *          but it will remain active.
     **/
    bool                mStarted;
    /**
     * \brief   Synchronization object
     **/
    CECriticalSection   mLock;

//////////////////////////////////////////////////////////////////////////
// Hidden methods
//////////////////////////////////////////////////////////////////////////
private:
    /**
     * \brief   Triggered in Timer Event object constructor, indicating that timer is queued.
     *          The function will increase the timer queued count and if it reaches
     *          its maximum, it will stop timer on Timer Manager side, 
     *          but will not reset timer data.
     *          The function will be ignored if maximum queue count in zero and/or is less
     *          than the event count.
     **/
    void TimerIsQueued( void );

    /**
     * \brief   Triggered in Timer Event object destructor, indicating that timer is removed from queue.
     *          The function will decrease the number of queue count and it is less than the maximum
     *          queue count, it will start timer using internal saved data.
     *          The function will be ignored if timer was stopped manually or it is completed to be fired.
     *          The function will be ignored if the maximum queue count is zero.
     **/
    void TimerIsUnqueued( void );

    /**
     * \brief   Returns reference to Timer object.
     **/
    inline CETimer & self( void );

//////////////////////////////////////////////////////////////////////////
// Forbidden calls
//////////////////////////////////////////////////////////////////////////
private:
    CETimer( void );
    CETimer(const CETimer & /*src*/);
    const CETimer& operator = (const CETimer & /*src*/ );
};

//////////////////////////////////////////////////////////////////////////
// CETimer class inline function implementation
//////////////////////////////////////////////////////////////////////////

inline CETimer & CETimer::self( void )
{   return (*this);                     }

inline const uint64_t& CETimer::GetFiredTime( void ) const
{   return mLastFired;                  }

inline IETimerConsumer& CETimer::GetTimerConsumer( void ) const
{   return mConsumer;                   }

inline const CEString & CETimer::GetTimerName( void ) const
{   return mName;                       }

inline unsigned int CETimer::GetFireTime( void ) const
{   return mTimeoutInMs;                }

inline unsigned int CETimer::GetTimeoutValue( void ) const
{   return mTimeoutInMs;                }

inline unsigned int CETimer::GetEventCount( void ) const
{   return mEventsCount;                }

inline bool CETimer::IsActive( void ) const
{   return (mEventsCount != 0);         }

inline bool CETimer::IsStopped( void ) const
{   return (mTimeoutInMs == 0);         }

inline int CETimer::GetTimePending( void ) const
{   return (IsActive() ? static_cast<int>(mNextFire - CETimer::GetTickCount()) : -1);   }

inline bool CETimer::HasName( void ) const
{   return (mName.IsEmpty() == false);  }

inline unsigned int CETimer::GetNextFireTime( void ) const
{   return mNextFire;                   }

#endif  // AREG_COMPONENT_CETIMER_HPP
