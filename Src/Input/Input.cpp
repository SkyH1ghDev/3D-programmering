#define KEY_PRESSED 0x8000

#include "Input.hpp"
#include "KeyboardConfig.hpp"

#include <iostream>


Input::Input()
{
    GetCursorPos(&this->_prevPoint);
    ShowCursor(FALSE);

    KeyboardConfig keyboardConfig;
    this->_keyboard = keyboardConfig.GetDefaultKeyboard();
}


void Input::ReadInput(Scene& scene, HWND& window, bool& running, MSG& msg, int& outputMode, const float& deltaTime)
{
    HandleKeyboard(scene, running, msg, outputMode, deltaTime);
    HandleRotation(scene.GetCurrentCamera(), window);
}

int Input::Exit(const MSG &msg)
{
    return !(GetAsyncKeyState(VK_ESCAPE) & KEY_PRESSED) && msg.message != WM_QUIT;
}

void Input::HandleKeyboard(Scene& scene, bool& running, MSG& msg, int& outputMode, const float& deltaTime)
{
    std::vector<std::shared_ptr<Key>> pressedKeys = this->_keyboard.GetPressedKeys();

    Camera& camera = scene.GetCurrentCamera();
    
    float movementAmount = 10.0f;
    
    for(std::shared_ptr<Key>& key : pressedKeys)
    {
        switch(key->GetAction())
        {
            case Unbound:
                break;
            
            case Quit:
                running = !key->IsPressed() && msg.message != WM_QUIT;
                break;
            
            case MoveUp:
                camera.MoveUp(movementAmount, deltaTime);
                break;

            case MoveDown:
                camera.MoveDown(movementAmount, deltaTime);
                break;

            case MoveForward:
                camera.MoveForward(movementAmount, deltaTime);
                break;

            case MoveBackward:
                camera.MoveBackward(movementAmount, deltaTime);
                break;

            case MoveLeft:
                camera.MoveLeft(movementAmount, deltaTime);
                break;

            case MoveRight:
                camera.MoveRight(movementAmount, deltaTime);
                break;

            case ToggleOutputMode:
                if (key->IsPressed() && !key->IsHeldDown())
                {
                    ++outputMode;
                    outputMode %= 7;
                    key->SetHoldDownState(true);
                }
                else if (!key->IsPressed())
                {
                    key->SetHoldDownState(false);
                }
                break;

            case ToggleMouseLock:
                if (key->IsPressed() && !key->IsHeldDown())
                {
                    this->_lockMouse = !this->_lockMouse;
                    key->SetHoldDownState(true);
                }
                else if (!key->IsPressed())
                {
                    key->SetHoldDownState(false);
                }
                break;

            case ToggleCamera:
                if (key->IsPressed() && !key->IsHeldDown())
                {
                    scene.IncrementCameraIndex();
                    key->SetHoldDownState(true);
                }
                else if (!key->IsPressed())
                {
                    key->SetHoldDownState(false);
                }
                break;

            default:
                break;
        }
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

    camera.ApplyRotation(-deltaPitch, deltaYaw);
    
    if (this->_lockMouse)
    {
        SetCursorPos(centerOfScreen.x, centerOfScreen.y);
    }

    this->_prevPoint = centerOfScreen;
}