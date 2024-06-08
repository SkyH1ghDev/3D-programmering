#pragma once
#include <chrono>

namespace CHRONO = std::chrono;

class Clock
{
public:
    Clock() = default;
    
    void Start();
    void End();
    float GetDeltaTime() const;

private:
    CHRONO::time_point<CHRONO::high_resolution_clock> _startTime;
    CHRONO::time_point<CHRONO::high_resolution_clock> _endTime;
};
