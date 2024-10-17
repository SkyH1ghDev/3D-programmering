#include "DepthBuffer.hpp"

#include "Configuration.hpp"
#include "D3D11Helper.hpp"
#include <stdexcept>
#include <iostream>


DepthBuffer::DepthBuffer(HRESULT& hr, ID3D11Device* device)
{
	WindowConfig windowConfig; 
	/*if (!d3d11Helper.CreateDepthStencil(device, windowConfig.GetWidth(), windowConfig.GetHeight(), this->_texture, this->_dsv, this->_srv))
	{
		throw std::runtime_error("Failed to Create DSV");
	}*/
	
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = windowConfig.GetWidth();
	textureDesc.Height = windowConfig.GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS; //DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	
	ID3D11Texture2D* texture;
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &texture)))
	{
		std::cerr << "Failed to create depth stencil texture!" << std::endl;
		hr = false;
		return;
	}

	this->_texture = texture;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	ID3D11DepthStencilView* dsv;
	if (FAILED(device->CreateDepthStencilView(texture, &dsvDesc, &dsv)))
	{
		std::cerr << "Failed to create depth stencil view" << std::endl;
		hr = false;
		return;
	}

	this->_dsv = dsv;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView* srv;
	if (FAILED(device->CreateShaderResourceView(texture, &srvDesc, &srv)))
	{
		std::cerr << "Failed to create shader resource view for depth stencil view" << std::endl;
		hr = false;
		return;
	}

	this->_srv = srv;
}

ID3D11ShaderResourceView* DepthBuffer::GetSRV() const
{
	return this->_srv.GetSRV();
}

ID3D11DepthStencilView* DepthBuffer::GetDSV() const
{
	return this->_dsv.GetDSV();
}



