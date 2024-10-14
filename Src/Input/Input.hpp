#pragma once

#include <windows.h>

#include "Scene.hpp"
#include "Keyboard.hpp"

class Input
{
public:
    Input();
    void ReadInput(Scene& scene, HWND& window, bool& running, MSG& msg, int& outputMode, const float& deltaTime);
    int Exit(const MSG& msg);
    
private:
    void HandleKeyboard(Scene& scene, bool& running, MSG& msg, int& outputMode, const float& deltaTime);
    void HandleRotation(Camera& camera, HWND& window);

private:
    POINT _prevPoint = POINT();
    bool _lockMouse = true;

    Keyboard _keyboard;

};
