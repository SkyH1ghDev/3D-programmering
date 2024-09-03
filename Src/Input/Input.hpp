#pragma once

#include <windows.h>

#include "Camera.hpp"

class Input
{
public:
    void ReadInput(Camera& camera, float deltaTime);
    int Exit(const MSG& msg);
    
private:
    void HandleMovement(Camera& camera, float deltaTime);
    void HandleRotation(Camera& camera, float deltaTime);
    
};
