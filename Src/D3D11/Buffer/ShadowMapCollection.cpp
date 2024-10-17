#include "ShadowMapCollection.hpp"

#include "WindowConfig.hpp"
#include <iostream>

ShadowMapCollection::ShadowMapCollection(HRESULT& hr, ID3D11Device* device, const UINT& arraySize)
{
	WindowConfig windowConfig;
	
	D3D11_TEXTURE2D_DESC depthBufferTextureDesc;
	depthBufferTextureDesc.Width = windowConfig.GetWidth();
	depthBufferTextureDesc.Height = windowConfig.GetHeight();
	depthBufferTextureDesc.MipLevels = 1;
	depthBufferTextureDesc.ArraySize = arraySize;
	depthBufferTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthBufferTextureDesc.SampleDesc.Count = 1;
	depthBufferTextureDesc.SampleDesc.Quality = 0;
	depthBufferTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferTextureDesc.CPUAccessFlags = 0;
	depthBufferTextureDesc.MiscFlags = 0;

	ID3D11Texture2D* depthBufferTexture;
	hr = device->CreateTexture2D(&depthBufferTextureDesc, nullptr, &depthBufferTexture);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create depth buffer texture");
	}

	this->_texture = depthBufferTexture;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthBufferDSVDesc;
	ZeroMemory(&depthBufferDSVDesc, sizeof(depthBufferDSVDesc));
	depthBufferDSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	depthBufferDSVDesc.Texture2DArray.MipSlice = 0;
	depthBufferDSVDesc.Texture2DArray.ArraySize = 1;

	std::vector<ID3D11DepthStencilView*> depthBufferDSV;
	
	for (UINT i = 0; i < arraySize; ++i)
	{
		depthBufferDSVDesc.Texture2DArray.FirstArraySlice = i;
		ID3D11DepthStencilView* dsv;
		hr = device->CreateDepthStencilView(depthBufferTexture, &depthBufferDSVDesc, &dsv);

		if (FAILED(hr))
		{
			throw std::runtime_error("Could not create depth buffer dsv");
		}

		this->_dsvs.push_back(dsv);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC depthBufferSRVDesc;
	ZeroMemory(&depthBufferSRVDesc, sizeof(depthBufferSRVDesc));

	depthBufferSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	depthBufferSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	depthBufferSRVDesc.Texture2DArray.MostDetailedMip = 0;
	depthBufferSRVDesc.Texture2DArray.MipLevels = 1;
	depthBufferSRVDesc.Texture2DArray.FirstArraySlice = 0;
	depthBufferSRVDesc.Texture2DArray.ArraySize = arraySize;

	ID3D11ShaderResourceView* srv;
	hr = device->CreateShaderResourceView(depthBufferTexture, &depthBufferSRVDesc, &srv);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create depth buffer srv");
	}

	this->_srv = srv;
}

DepthStencilView ShadowMapCollection::GetDepthStencilViewAt(UINT index)
{
	return this->_dsvs.at(index);
}

ShaderResourceView ShadowMapCollection::GetShaderResourceView()
{
	return this->_srv;
}

Texture2D ShadowMapCollection::GetTexture2DArray()
{
	return this->_texture;
}


