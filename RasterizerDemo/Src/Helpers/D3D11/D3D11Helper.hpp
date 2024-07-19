#pragma once

#include <Windows.h>
#include <d3d11.h>

class D3D11Helper
{
public:
	D3D11Helper() = default;
	
	/*bool SetupD3D11(HWND window, ID3D11Device*& device,
		ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain, ID3D11RenderTargetView*& rtv,
		ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport);*/
	bool CreateController(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext);
	bool CreateSwapChain(ID3D11Device*, IDXGISwapChain*& swapChain, ID3D11Texture2D*& backBuffer, ID3D11UnorderedAccessView*& uav, HWND window, UINT width, UINT height);
	void SetViewport(D3D11_VIEWPORT& viewport, UINT width, UINT height);
	bool CreateRenderTargetView(ID3D11Device* device, ID3D11Texture2D*& backBuffer, ID3D11RenderTargetView*& rtv);
	bool CreateGBuffer(ID3D11Device* device, ID3D11RenderTargetView*& rtv, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& srv, UINT width, UINT height);
    bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, ID3D11ShaderResourceView*& srv);
};