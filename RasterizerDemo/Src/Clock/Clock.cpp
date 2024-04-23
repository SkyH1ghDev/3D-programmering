#include "Clock.hpp"

void Clock::Start()
{
    this->_startTime = CHRONO::high_resolution_clock::now();
}

void Clock::End()
{
    this->_endTime = CHRONO::high_resolution_clock::now();
}

float Clock::GetDeltaTime()
{
    CHRONO::duration<float> timeDiff = this->_endTime - this->_startTime;
    return timeDiff.count();
}


