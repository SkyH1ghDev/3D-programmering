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
    bool _outputModeKeyHeldDown = false;
    bool _lockMouseKeyHeldDown = false;

    int _upKey;
    int _downKey;
    int _forwardKey;
    int _backKey;
    int _leftKey;
    int _rightKey;
    
    int _quitKey;
    int _outputModeKey;
    int _lockMouseKey;
};
