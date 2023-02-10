#include "HarnessSleep.h"
#define _WIN32_WINNT 0x0500


HANDLE gDoneEvent;


/*
void CALLBACK doNothing(PVOID lpParam, BOOLEAN TimerOrWaitFired) { SetEvent(gDoneEvent); }
*/


void harnessSleep(long duration) {
    //obtain high resolution clock
    TIMECAPS tc;
    UINT     wTimerRes;

    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
    {
        // Error; application can't continue.
    }
    wTimerRes = tc.wPeriodMin;
    timeBeginPeriod(wTimerRes);

    Sleep((DWORD)duration);

   /* HANDLE hTimer = nullptr, hTimerQueue = nullptr;
    //int arg = (int)duration;
    // Use an event object to track the TimerRoutine execution
    gDoneEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (!gDoneEvent)
    {
        printf("CreateEvent failed (%d)\n", GetLastError());
        return;
    }
    // Create the timer queue.
    hTimerQueue = CreateTimerQueue();
    if (!hTimerQueue)
    {
        printf("CreateTimerQueue failed (%d)\n", GetLastError());
        return;
    }
    if (!CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)doNothing,
        nullptr, (DWORD)duration, 0, 0))
    {
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
        return;
    }

    // Wait for the timer-queue thread to complete using an event 
    // object. The thread will signal the event at that time.

    if (WaitForSingleObject(gDoneEvent, INFINITE) != WAIT_OBJECT_0)
        printf("WaitForSingleObject failed (%d)\n", GetLastError());



    CloseHandle(gDoneEvent);

    //if (!DeleteTimerQueue(hTimerQueue))
    //   printf("DeleteTimerQueue failed (%d)\n", GetLastError());
    //timeEndPeriod(tc.wPeriodMin);
    */
}


