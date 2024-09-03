#include "D3D11Helper.hpp"

#include <iostream>

#include "WindowConfig.hpp"

bool D3D11Helper::CreateController(ID3D11Device*& device, ID3D11DeviceContext*& immediateContext)
{
	UINT flags = 0;
	if (_DEBUG)
		flags = D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };



	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, 1, D3D11_SDK_VERSION, &device, nullptr, &immediateContext);
	
	return !(FAILED(hr));
}

bool D3D11Helper::CreateSwapChain(ID3D11Device* device, IDXGISwapChain*& swapChain, ID3D11Texture2D*& backBuffer, ID3D11UnorderedAccessView*& uav, HWND window, UINT width, UINT height)
{
	IDXGIFactory* factory = nullptr;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

	if (FAILED(hr))
	{
		return false;
	}
	
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Default
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hr = factory->CreateSwapChain(device, &swapChainDesc, &swapChain);

	if (FAILED(hr))
	{
		std::cerr << "Failed to Create Swap Chain \n";
		return false;
	}
	
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	
	if (FAILED(hr))
	{
		std::cerr << "Failed to get back buffer!" << std::endl;
		return false;
	}

	hr = device->CreateUnorderedAccessView(backBuffer, nullptr, &uav);

	return !(FAILED(hr));
}

bool D3D11Helper::CreateRenderTargetView(ID3D11Device* device, ID3D11Texture2D*& backBuffer, ID3D11RenderTargetView*& rtv)
{
	HRESULT hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);
	return !(FAILED(hr));
}

bool D3D11Helper::CreateGBuffer(ID3D11Device* device, ID3D11RenderTargetView*& rtv, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& srv, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &texture)))
	{
		std::cerr << "Failed to create GBuffer Texture\n";
		return false;
	}

	if (FAILED(device->CreateShaderResourceView(texture, nullptr, &srv)))
	{
		std::cerr << "Failed to create GBuffer Shader Resource View\n";
		return false;
	}

	if (FAILED(device->CreateRenderTargetView(texture, nullptr, &rtv)))
	{
		std::cerr << "Failed to create GBuffer Render Target View\n"; 
		return false;
	}

	return true;
}


bool D3D11Helper::CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, ID3D11ShaderResourceView*& srv)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS; 
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		std::cerr << "Failed to create depth stencil texture!" << std::endl;
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	
	if (FAILED(device->CreateDepthStencilView(dsTexture, &dsvDesc, &dsView)))
	{
		std::cerr << "Failed to create depth stencil view" << std::endl;
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	
	if (FAILED(device->CreateShaderResourceView(dsTexture, &srvDesc, &srv)))
	{
		std::cerr << "Failed to create shader resource view for depth stencil view" << std::endl;
		return false;
	}
	return true;
}

void D3D11Helper::SetViewport(D3D11_VIEWPORT& viewport, UINT width, UINT height)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}