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
    HWND _window;
    Controller _controller;
    SwapChain _swapChain;
    RenderTargetView _windowRTV;
    std::vector<RenderTargetView> _gBuffers;
    Scene _scene;

    /*
     * Forward Rendering
     */

    std::unique_ptr<VertexShader> _vsForward;
    std::unique_ptr<PixelShader> _psForward;
    
    /*
     * Deferred Rendering
     */
    
    std::unique_ptr<VertexShader> _vsDeferredGeometry;
    std::unique_ptr<PixelShader> _psDeferredGeometry;
    //std::unique_ptr<HullShader> _hullShader;
    //std::unique_ptr<DomainShader> _domainShader;
    //std::unique_ptr<GeometryShader> _geometryShader;
    std::unique_ptr<ComputeShader> _csDeferredLight;
    
    InputLayout _inputLayout;
    Sampler _sampler;
    Input _input;
    Clock _clock;
    Renderer _renderer;
    MSG _msg = {};

    int _outputMode = 0;
};
