#pragma once
#include <Arduino.h>

class SimpleTimer
{     
    bool timerState;  
    uint16 timerInterval;
    unsigned long timerTimeoutMillis;
    void (*timerTimeoutEvent)();

    public:          
        SimpleTimer(uint16 x, void (*foo)());
        void start();
        void stop();
        void check();
        void changeTimeout(uint16 x);
};
