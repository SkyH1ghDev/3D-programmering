#include "Application.hpp"

#include "SetupHelper.hpp"

int Application::Run(HINSTANCE hInstance, int nCmdShow)
{
    Setup(hInstance, nCmdShow); 
    Render();

    return 0;
}

void Application::Setup(HINSTANCE hInstance, int nCmdShow)
{
    // Setup Window
    this->_window = SetupHelper::SetupWindow(hInstance, nCmdShow);
    
    this->_controller = SetupHelper::SetupController(this->_window);
    this->_rtv = SetupHelper::SetupRenderTarget(this->_controller);

    
    
}

void Application::Render()
{
    
}



