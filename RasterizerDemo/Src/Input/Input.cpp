#define KEY_PRESSED 0x8000

#include "Input.hpp"

#include <iostream>

void Input::ReadInput(Camera& camera, float deltaTime)
{
    HandleMovement(camera, deltaTime);
    HandleRotation(camera, deltaTime);
}

int Input::Exit(const MSG &msg)
{
    return !(GetAsyncKeyState(VK_ESCAPE) & KEY_PRESSED) && msg.message != WM_QUIT;
}


void Input::HandleMovement(Camera& camera, float deltaTime)
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

void Input::HandleRotation(Camera &camera, float deltaTime)
{
    float angle = DX::XMConvertToRadians(30.0f);
    
    if (GetAsyncKeyState(VK_UP) & KEY_PRESSED)
    {
        camera.RotateLeft(angle, deltaTime);
    }

    if (GetAsyncKeyState(VK_DOWN) & KEY_PRESSED)
    {
        camera.RotateRight(angle, deltaTime);
    }

    if (GetAsyncKeyState(VK_LEFT) & KEY_PRESSED)
    {
        camera.RotateDown(angle, deltaTime);
    }

    if (GetAsyncKeyState(VK_RIGHT) & KEY_PRESSED)
    {
        camera.RotateUp(angle, deltaTime);
    }
}
