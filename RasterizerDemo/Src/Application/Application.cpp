﻿#include "Application.hpp"


Application::Application(HINSTANCE hInstance, int nCmdShow) :
    // Initialize Window
    _window(Setup::SetupWindow(hInstance, nCmdShow)),

    // Initialize D3D11
    _controller(Setup::SetupController(this->_window)),
    _rtv(Setup::SetupRenderTarget(this->_controller)),
    _scene(Setup::SetupScene(this->_controller)),

    // Initialize Shaders
    _vertexShader(Setup::SetupShader(this->_controller, ShaderType::VERTEX_SHADER, L"Src/Shaders/VertexShader.hlsl")),
    //_hullShader();
    //_domainShader(),
    //_geometryShader(),
    _pixelShader(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"Src/Shaders/PixelShader.hlsl")),
    //_computeShader()

    _inputLayout(Setup::SetupInputLayout(this->_controller, this->_vertexShader))
{
    // Window
    assert(&this->_window != nullptr);

    // Controller
    assert(this->_controller.GetContext() != nullptr);
    assert(this->_controller.GetDevice() != nullptr);
    assert(this->_controller.GetSwapChain() != nullptr);

    D3D11_VIEWPORT vp = this->_controller.GetViewPort();
    assert(&vp != nullptr);

    // RTV
    assert(this->_rtv.GetDSV() != nullptr);
    assert(this->_rtv.GetRTV() != nullptr);

    //Scene
    /*
     * TODO: Implement asserts for scene, mesh, submesh and camera
    */
    
    // Shaders
    assert(this->_vertexShader.GetShaderBlob() != nullptr);
    assert(this->_pixelShader.GetShaderBlob() != nullptr);

    // Input layout
    assert(this->_inputLayout.GetInputLayout() != nullptr);
}

int Application::Run()
{
    Setup(); 
    Render();

    return 0;
}

void Application::Setup()
{
    
}

void Application::Render()
{
    
}



