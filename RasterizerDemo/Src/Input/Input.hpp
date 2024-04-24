#pragma once

#include <windows.h>

#include "Camera.hpp"

class Input
{
public:
    void ReadInput(const MSG& msg, Camera& camera, float deltaTime);
    
private:
    void handleMovement(const MSG& msg, Camera& camera, float deltaTime);
    
};
