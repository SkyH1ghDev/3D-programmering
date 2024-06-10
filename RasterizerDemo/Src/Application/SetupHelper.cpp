#include "SetupHelper.hpp"

#include <iostream>
#include "WindowHelper.hpp"
#include "D3D11Helper.hpp"
#include "Configuration.hpp"

HWND SetupHelper::SetupWindow(HINSTANCE hInstance, int nCmdShow) 
{
	HWND window;
	WindowHelper windowHelper;
	if (!windowHelper.SetupWindow(hInstance, nCmdShow, window))
	{
		std::cerr << "Failed to setup window! \n Exiting... \n";
		exit(0);	
	}

	return window;
}

D3D11Controller SetupHelper::SetupController(HWND window)
{
    D3D11Helper d3d11Helper;
    Configuration config;

    WindowConfig windowConfig = config.GetWindowConfig();
    UINT height = windowConfig.GetHeight();
    UINT width = windowConfig.GetWidth();

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	D3D11_VIEWPORT viewport;
	
	if (!d3d11Helper.CreateInterfaces(device, immediateContext, swapChain, width, height, window))
	{
		std::cerr << "Failed to setup Device, ImmediateContext and SwapChain \n Exiting... \n";
		exit(0);
	}
	
    d3d11Helper.SetViewport(viewport, width, height);

	return D3D11Controller(device, immediateContext, swapChain, viewport);
}

