#define KEY_PRESSED 0x8000

#include "Input.hpp"

#include <iostream>

Input::Input()
{
    GetCursorPos(&this->_prevPoint);
    ShowCursor(FALSE);
}


void Input::ReadInput(Camera& camera, HWND& window, int& outputMode, const float& deltaTime)
{
    HandleMovement(camera, deltaTime);
    HandleRotation(camera, window);
    HandleMiscellaneous(outputMode);
}

int Input::Exit(const MSG &msg)
{
    return !(GetAsyncKeyState(VK_ESCAPE) & KEY_PRESSED) && msg.message != WM_QUIT;
}


void Input::HandleMovement(Camera& camera, const float& deltaTime)
{
    float movementAmount = 10.0f;

    if (GetAsyncKeyState(VK_SPACE) & KEY_PRESSED)
    {
        camera.MoveUp(movementAmount, deltaTime);
    }

    if (GetAsyncKeyState(VK_LCONTROL) & KEY_PRESSED)
    {
        camera.MoveDown(movementAmount, deltaTime);
    }
    
    if (GetAsyncKeyState('W') & KEY_PRESSED)
    {
        camera.MoveForward(movementAmount, deltaTime);
    }

    if (GetAsyncKeyState('S') & KEY_PRESSED)
    {
        camera.MoveBackward(movementAmount, deltaTime);
    }
    
    if (GetAsyncKeyState('A') & KEY_PRESSED)
    {
        camera.MoveLeft(movementAmount, deltaTime);
    }

    if (GetAsyncKeyState('D') & KEY_PRESSED)
    {
        camera.MoveRight(movementAmount, deltaTime);
    }
}

void Input::HandleRotation(Camera& camera, HWND& window)
{
    WindowConfig windowConfig;
    constexpr float radiansPerPixel = DX::XMConvertToRadians(1.0f);
    constexpr float sensitivity = 0.1f;

    
    POINT centerOfScreen = POINT(windowConfig.GetWidth() / 2, windowConfig.GetHeight() / 2);
    ClientToScreen(window, &centerOfScreen);

    POINT currPoint;
    if (this->_lockMouse)
    {
        GetCursorPos(&currPoint);
    }
    else
    {
        currPoint = centerOfScreen;
    }

    float deltaYaw = static_cast<float>(currPoint.x - this->_prevPoint.x) * sensitivity * radiansPerPixel;
    float deltaPitch = static_cast<float>(currPoint.y - this->_prevPoint.y) * sensitivity * radiansPerPixel;

    camera.ApplyRotation(deltaPitch, deltaYaw);
    


    if (this->_lockMouse)
    {
        SetCursorPos(centerOfScreen.x, centerOfScreen.y);
    }

    this->_prevPoint = centerOfScreen;
}

void Input::HandleMiscellaneous(int& outputMode)
{
    if (GetAsyncKeyState('M') & KEY_PRESSED)
    {
        this->_lockMouse = !this->_lockMouse;
    }

    if (int outputModeKeyState = GetAsyncKeyState('N'); outputModeKeyState & KEY_PRESSED && !this->_outputModeKeyHeldDown)
    {
        ++outputMode;
        outputMode %= 7;
        this->_outputModeKeyHeldDown = true;
    }
    else if (!(outputModeKeyState & KEY_PRESSED))
    {
        this->_outputModeKeyHeldDown = false;
    }
}
