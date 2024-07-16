#pragma once

#include <windows.h>
#include <xstring>

#include "D3D11Controller.hpp"
#include "InputLayout.hpp"
#include "RenderTargetView.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

class Setup
{
public:
    
    static HWND SetupWindow(HINSTANCE hInstance, int nCmdShow);
    static D3D11Controller SetupController(HWND window);
    static RenderTargetView SetupRenderTargetView(D3D11Controller& controller);
    static RenderTargetView SetupGBuffer(D3D11Controller& controller);
    static Scene SetupScene(D3D11Controller& controller);
    static Shader* SetupShader(D3D11Controller& controller, ShaderType shaderType, std::wstring hlslFilename);
    static InputLayout SetupInputLayout(D3D11Controller& controller, const Shader& vertexShader);
    static Sampler SetupSampler(D3D11Controller& controller);
    static ConstantBuffer CreateWorldMatrixConstantBuffer(D3D11Controller& controller);
    static ConstantBuffer CreatePixelShaderConstantBuffer(D3D11Controller& controller, Scene& scene);

private:

    static void AppendLPCWSTR(LPCWSTR& newStr, LPCWSTR first, LPCWSTR second);
};
