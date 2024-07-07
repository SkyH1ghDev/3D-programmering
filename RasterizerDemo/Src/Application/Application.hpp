#pragma once

#include "VertexShader.hpp"
#include "HullShader.hpp"
#include "DomainShader.hpp"
#include "GeometryShader.hpp"
#include "PixelShader.hpp"
#include "ComputeShader.hpp"

#include "Input.hpp"
#include "Clock.hpp"
#include "Renderer.hpp"

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
    RenderTarget _windowRTV;
    Scene _scene; // Could be expanded to contain multiple scenes

    // Shaders
    std::unique_ptr<VertexShader> _vertexShaderPtr;
    //std::unique_ptr<HullShader> _hullShader;
    //std::unique_ptr<DomainShader> _domainShader;
    //std::unique_ptr<GeometryShader> _geometryShader;
    std::unique_ptr<PixelShader> _pixelShaderPtr;
    //std::unique_ptr<ComputeShader> _computeShader;

    // Input Layout and Sampler
    InputLayout _inputLayout;
    Sampler _sampler;

    ConstantBuffer _worldMatrixConstantBuffer;
    ConstantBuffer _pixelShaderConstantBuffer;

    Input _input;
    Clock _clock;
    Renderer _renderer;
    MSG _msg = {};
};
