#include "CubeMapCollection.hpp"

#include <iostream>

CubeMapCollection::CubeMapCollection(HRESULT& hr, ID3D11Device* device)
{
    
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = 2048;
	textureDesc.Height = 2048;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 6;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* texture;
	hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create texture cube");
	}

	this->_texture = texture;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.ArraySize = 1;
	uavDesc.Texture2DArray.MipSlice = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2DArray.ArraySize = 1;
	rtvDesc.Texture2DArray.MipSlice = 0;
	
	for (int i = 0; i < 6; ++i)
	{
		ID3D11UnorderedAccessView* uav;
		
		uavDesc.Texture2DArray.FirstArraySlice = i;
		hr = device->CreateUnorderedAccessView(texture, &uavDesc, &uav);

		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to create cube map uav");
		}

		this->_uavs.push_back(uav);

		ID3D11RenderTargetView* rtv;

		rtvDesc.Texture2DArray.FirstArraySlice = i;
		hr = device->CreateRenderTargetView(texture, &rtvDesc, &rtv);

		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to create cube map rtv");
		}

		this->_rtvs.push_back(rtv);
	}

	ID3D11ShaderResourceView* srv;
	hr = device->CreateShaderResourceView(texture, nullptr, &srv);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create cube map srv");
	}

	this->_srv = srv;
}

Texture2D CubeMapCollection::GetTexture2DArray()
{
    return this->_texture;
}

ShaderResourceView CubeMapCollection::GetShaderResourceView()
{
    return this->_srv;
}

UnorderedAccessView CubeMapCollection::GetUnorderedAccessViewAt(UINT index)
{
    return this->_uavs.at(index);
}

RenderTargetView CubeMapCollection::GetRenderTargetViewAt(UINT index)
{
	return this->_rtvs.at(index);
}



