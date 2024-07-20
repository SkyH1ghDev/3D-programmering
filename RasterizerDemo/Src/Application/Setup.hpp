#pragma once

#include <windows.h>
#include <xstring>

#include "Controller.hpp"
#include "InputLayout.hpp"
#include "RenderTargetView.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "SwapChain.hpp"

class Setup
{
public:
    
    static HWND SetupWindow(HINSTANCE hInstance, int nCmdShow);
    static Controller SetupController(HWND window);
    static SwapChain SetupSwapChain(Controller& controller, HWND window);
    static RenderTargetView SetupRenderTargetView(Controller& controller, SwapChain& swapChain);
    static RenderTargetView SetupGBuffer(Controller& controller);
    static Scene SetupScene(Controller& controller);
    static Shader* SetupShader(Controller& controller, ShaderType shaderType, std::wstring hlslFilename);
    static InputLayout SetupInputLayout(Controller& controller, const Shader& vertexShader);
    static Sampler SetupSampler(Controller& controller);
    static ConstantBuffer CreateWorldMatrixConstantBuffer(Controller& controller);
    static ConstantBuffer CreatePixelShaderConstantBuffer(Controller& controller, Scene& scene);
};
