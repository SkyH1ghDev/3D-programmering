#define KEY_PRESSED 0x8000

#include "Input.hpp"

#include <iostream>

#include "InputConfig.hpp"

Input::Input()
{
    GetCursorPos(&this->_prevPoint);
    ShowCursor(FALSE);

    InputConfig inputConfig;

    this->_quitKey = inputConfig.GetQuitKey();

    this->_upKey = inputConfig.GetUpKey();
    this->_downKey = inputConfig.GetDownKey();
    this->_forwardKey = inputConfig.GetForwardKey();
    this->_backKey = inputConfig.GetBackKey();
    this->_leftKey = inputConfig.GetLeftKey();
    this->_rightKey = inputConfig.GetRightKey();

    this->_outputModeKey = inputConfig.GetOutputModeKey();
    this->_lockMouseKey = inputConfig.GetLockMouseKey();
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

    if (GetAsyncKeyState(this->_upKey) & KEY_PRESSED)
    {
        camera.MoveUp(movementAmount, deltaTime);
    }

    if (GetAsyncKeyState(this->_downKey) & KEY_PRESSED)
    {
        camera.MoveDown(movementAmount, deltaTime);
    }
    
    if (GetAsyncKeyState(this->_forwardKey) & KEY_PRESSED)
    {
        camera.MoveForward(movementAmount, deltaTime);
    }

    if (GetAsyncKeyState(this->_backKey) & KEY_PRESSED)
    {
        camera.MoveBackward(movementAmount, deltaTime);
    }
    
    if (GetAsyncKeyState(this->_leftKey) & KEY_PRESSED)
    {
        camera.MoveLeft(movementAmount, deltaTime);
    }

    if (GetAsyncKeyState(this->_rightKey) & KEY_PRESSED)
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
        ShowCursor(FALSE);
    }
    else
    {
        currPoint = centerOfScreen;
        ShowCursor(TRUE);
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
    if (int lockMouseKeyState = GetAsyncKeyState(this->_lockMouseKey); lockMouseKeyState & KEY_PRESSED && !this->_lockMouseKeyHeldDown)
    {
        this->_lockMouse = !this->_lockMouse;
        this->_lockMouseKeyHeldDown = true;
    }
    else if (!(lockMouseKeyState & KEY_PRESSED))
    {
        this->_lockMouseKeyHeldDown = false;
    }

    if (int outputModeKeyState = GetAsyncKeyState(this->_outputModeKey); outputModeKeyState & KEY_PRESSED && !this->_outputModeKeyHeldDown)
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
