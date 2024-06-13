#pragma once

#include "Setup.hpp"

class Application
{
public:
    Application(HINSTANCE hInstance, int nCmdShow);
    ~Application() = default;
    int Run();

private:
    void Setup();
    void Render();

private:
    // Window
    HWND _window;

    // D3D11
    D3D11Controller _controller;
    RenderTarget _rtv;
    Scene _scene; // Could be expanded to contain multiple scenes

    // Shaders
    Shader _vertexShader;
    //Shader _hullShader;
    //Shader _domainShader;
    //Shader _geometryShader;
    Shader _pixelShader;
    //Shader _computeShader;
};
