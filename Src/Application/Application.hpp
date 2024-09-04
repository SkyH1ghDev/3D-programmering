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
#include "SwapChain.hpp"

class Application
{
public:
    Application(HINSTANCE hInstance, int nCmdShow);
    ~Application() = default;
    int Run();

private:
    void Setup();
    void SetupForwardBuffers();
    void SetupDeferredBuffers();
    
    void Render();
    void RunAsserts();

private:
    // Window
    HWND _window;

    // D3D11
    Controller _controller;
    SwapChain _swapChain;
    RenderTargetView _windowRTV;
    std::vector<RenderTargetView> _gBuffers;
    Scene _scene; // Could be expanded to contain multiple scenes

    // Shaders

    /*
     * Forward Rendering
     */

    std::unique_ptr<VertexShader> _vsForward;
    std::unique_ptr<PixelShader> _psForward;
    
    /*
     * Deferred Rendering
     */
    
    // Geometry Pass
    std::unique_ptr<VertexShader> _vsDeferredGeometry;
    std::unique_ptr<PixelShader> _psDeferredGeometry;
    
    //std::unique_ptr<HullShader> _hullShader;
    //std::unique_ptr<DomainShader> _domainShader;
    //std::unique_ptr<GeometryShader> _geometryShader;

    // Light Pass
    
    std::unique_ptr<ComputeShader> _csDeferredLight;
    
    //std::unique_ptr<HullShader> _hullShader;
    //std::unique_ptr<DomainShader> _domainShader;
    //std::unique_ptr<GeometryShader> _geometryShader;
    //std::unique_ptr<ComputeShader> _computeShader;

    // Input Layout and Sampler
    InputLayout _inputLayout;
    Sampler _sampler;

    ConstantBuffer _worldMatrixConstantBuffer;
    ConstantBuffer _lightingConstants;

    Input _input;
    Clock _clock;
    Renderer _renderer;
    MSG _msg = {};
};
