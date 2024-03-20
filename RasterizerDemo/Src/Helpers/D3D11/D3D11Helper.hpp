#pragma once

#include <Windows.h>
#include <d3d11.h>

class D3D11Helper
{
public:
	D3D11Helper() = default;
	
	bool SetupD3D11(UINT width, UINT height, HWND window, ID3D11Device*& device,
		ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain, ID3D11RenderTargetView*& rtv,
		ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport);
	
private:
	bool CreateInterfaces(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain, UINT width, UINT height, HWND window);
	bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& rtv);
    bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView);
    void SetViewport(D3D11_VIEWPORT& viewport, UINT width, UINT height);
};