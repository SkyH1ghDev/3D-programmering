#pragma once
#include <windows.h>
#include "D3D11Controller.hpp"
#include "RenderTarget.hpp"

class Application
{
public:
    int Run(HINSTANCE hInstance, int nCmdShow);

private:
    void Setup(HINSTANCE hInstance, int nCmdShow);
    void Render();

private:
    // Window
    HWND _window;

    // D3D11
    D3D11Controller _controller;
    RenderTarget _rtv;
};
