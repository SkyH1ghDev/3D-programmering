#include "Application.hpp"


Application::Application(HINSTANCE hInstance, int nCmdShow) :
    // Initialize Window
    _window(Setup::SetupWindow(hInstance, nCmdShow)),

    // Initialize D3D11
    _controller(Setup::SetupController(this->_window)),
    _rtv(Setup::SetupRenderTarget(this->_controller)),
    _scene(Setup::SetupScene(this->_controller)),

    // Initialize Shaders
    _vertexShader(Setup::SetupShader(this->_controller, ShaderType::VERTEX_SHADER, L"Build/VertexShader.cso")),
    //_hullShader();
    //_domainShader(),
    //_geometryShader(),
    _pixelShader(Setup::SetupShader(this->_controller, ShaderType::PIXEL_SHADER, L"Build/PixelShader.cso"))
    //_computeShader()
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



