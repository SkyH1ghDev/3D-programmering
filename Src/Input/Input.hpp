#pragma once

#include <windows.h>

#include "Camera.hpp"

class Input
{
public:
    Input();
    void ReadInput(Camera& camera, HWND& window, float deltaTime);
    int Exit(const MSG& msg);
    
private:
    void HandleMovement(Camera& camera, float deltaTime);
    void HandleRotation(Camera& camera, HWND& window, float deltaTime);

private:
    POINT _prevPoint = POINT();
    
};
