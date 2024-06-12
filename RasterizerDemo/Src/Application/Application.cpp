#include "Application.hpp"


Application::Application(HINSTANCE hInstance, int nCmdShow) :
    // Initialize Window
    _window(SetupHelper::SetupWindow(hInstance, nCmdShow)),

    // Initialize D3D11
    _controller(SetupHelper::SetupController(this->_window)),
    _rtv(SetupHelper::SetupRenderTarget(this->_controller)),
    _scene(SetupHelper::SetupScene(this->_controller)),

    // Initialize Shaders
    _vertexShader(),
    _hullShader(),
    _domainShader(),
    _geometryShader(),
    _pixelShader(),
    _computeShader()
{
    
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



