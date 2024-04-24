#include "Input.hpp"

void Input::ReadInput(const MSG& msg, Camera& camera, float deltaTime)
{
    if (msg.message == WM_KEYDOWN)
    {
        handleMovement(msg, camera, deltaTime);
    }
}
void Input::handleMovement(const MSG& msg, Camera& camera, float deltaTime)
{
    switch (msg.wParam)
    {
        case VK_SPACE:
            camera.MoveUp(10.0f, deltaTime);
            break;
        
        case VK_LCONTROL:
            camera.MoveDown(10.0f, deltaTime);
            break;

        case VK_UP:
            camera.MoveForward(10.0f, deltaTime);
            break;
        
        case VK_DOWN:
            camera.MoveBackward(10.0f, deltaTime);
            break;
        
        case VK_LEFT:
            camera.MoveLeft(10.0f, deltaTime);
            break;
        
        case VK_RIGHT:
            camera.MoveRight(10.0f, deltaTime);
            break;
        
        default:
            return;
    }
}
