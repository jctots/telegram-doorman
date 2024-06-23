#include "SimpleTimer.h"

SimpleTimer::SimpleTimer(uint16 x, void (*foo)())
{
    timerTimeoutEvent = foo;
    timerInterval = x;
    timerState = LOW; //stopped
}

void SimpleTimer::start()
{
    timerState = HIGH; //running
    timerTimeoutMillis = millis()+timerInterval;
}

void SimpleTimer::stop()
{
    timerState = LOW; //stopped
}
  
void SimpleTimer::check()
{
    if ((timerState == HIGH) && (millis() >= timerTimeoutMillis)) {
        timerState = LOW; //stopped
        (*timerTimeoutEvent)(); //execute timer event
        
    }
}

void SimpleTimer::changeTimeout(uint16 x)
{
   timerInterval = x;
}

