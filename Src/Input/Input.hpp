#pragma once

#include <windows.h>

#include "Camera.hpp"

class Input
{
public:
    Input();
    void ReadInput(Camera& camera, HWND& window, int& outputMode, const float& deltaTime);
    int Exit(const MSG& msg);
    
private:
    void HandleMovement(Camera& camera, const float& deltaTime);
    void HandleRotation(Camera& camera, HWND& window);
    void HandleMiscellaneous(int& outputMode);

private:
    POINT _prevPoint = POINT();
    bool _lockMouse = true;
    bool _outputModeKeyHeldDown = true;
};
