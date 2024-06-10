#pragma once

#include <Windows.h>
#include <d3d11.h>

class D3D11Helper
{
public:
	D3D11Helper() = default;
	
	bool SetupD3D11(HWND window, ID3D11Device*& device,
		ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain, ID3D11RenderTargetView*& rtv,
		ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport);
	bool CreateInterfaces(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain, UINT width, UINT height, HWND window);
	void SetViewport(D3D11_VIEWPORT& viewport, UINT width, UINT height);
	bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& rtv);
	
private:
    bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView);
    
};