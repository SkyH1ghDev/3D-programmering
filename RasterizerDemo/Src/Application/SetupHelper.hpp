#pragma once

#include <windows.h>

#include "D3D11Controller.hpp"
#include "RenderTarget.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

class SetupHelper
{
public:
    
    static HWND SetupWindow(HINSTANCE hInstance, int nCmdShow);
    static D3D11Controller SetupController(HWND window);
    static RenderTarget SetupRenderTarget(D3D11Controller& controller);
    static Scene SetupScene(D3D11Controller& controller);
    static Shader SetupShader(ShaderType shaderType);
};
