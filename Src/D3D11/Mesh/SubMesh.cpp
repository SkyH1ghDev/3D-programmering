#include "SubMesh.hpp"

#include "ImageData.hpp"
#include <iostream>

SubMesh::SubMesh(HRESULT& hr, ID3D11Device* device, const SubMeshInfo& subMeshInfo)
{
    this->_startIndex = subMeshInfo.StartIndexValue;
    this->_nrOfIndices = subMeshInfo.NrOfIndicesInSubMesh;
    this->_specularExponent = subMeshInfo.MTLData.specularExponent;

	// AMBIENT
	ImageData ambientData = subMeshInfo.MTLData.ambientTextureData;
    	
	D3D11_TEXTURE2D_DESC ambientTexture2DDesc;
	ambientTexture2DDesc.Width = static_cast<UINT>(ambientData.Width);
	ambientTexture2DDesc.Height = static_cast<UINT>(ambientData.Height);
	ambientTexture2DDesc.MipLevels = 1;
	ambientTexture2DDesc.ArraySize = 1;
	ambientTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ambientTexture2DDesc.SampleDesc = DXGI_SAMPLE_DESC{1, 0};
	ambientTexture2DDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ambientTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	ambientTexture2DDesc.CPUAccessFlags = 0;
	ambientTexture2DDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ambientTexture2DData;
	ambientTexture2DData.pSysMem = ambientData.TextureData.data(); 
	ambientTexture2DData.SysMemPitch = static_cast<UINT>(ambientData.Width) * ambientData.RGBAChannels;
	ambientTexture2DData.SysMemSlicePitch = 0;

	ID3D11Texture2D* ambientTex;
	hr = device->CreateTexture2D(&ambientTexture2DDesc, &ambientTexture2DData, &ambientTex);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Ambient Texture2D \n";
	}

	this->_ambientTexture2D = ambientTex;

	ID3D11ShaderResourceView* ambientTexSRV;
	device->CreateShaderResourceView(ambientTex, nullptr, &ambientTexSRV);

	this->_ambientTextureSRV = ambientTexSRV;
	
	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Ambient SRV \n";
	}
	
	// DIFFUSE
	ImageData diffuseData = subMeshInfo.MTLData.diffuseTextureData;
			
	D3D11_TEXTURE2D_DESC diffuseTexture2DDesc;
	diffuseTexture2DDesc.Width = static_cast<UINT>(diffuseData.Width);
	diffuseTexture2DDesc.Height = static_cast<UINT>(diffuseData.Height);
	diffuseTexture2DDesc.MipLevels = 1;
	diffuseTexture2DDesc.ArraySize = 1;
	diffuseTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	diffuseTexture2DDesc.SampleDesc = DXGI_SAMPLE_DESC{1, 0};
	diffuseTexture2DDesc.Usage = D3D11_USAGE_IMMUTABLE;
	diffuseTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	diffuseTexture2DDesc.CPUAccessFlags = 0;
	diffuseTexture2DDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA diffuseTexture2DData;
	diffuseTexture2DData.pSysMem = diffuseData.TextureData.data(); 
	diffuseTexture2DData.SysMemPitch = static_cast<UINT>(diffuseData.Width) * diffuseData.RGBAChannels;
	diffuseTexture2DData.SysMemSlicePitch = 0;

	ID3D11Texture2D* diffuseTex;
	hr = device->CreateTexture2D(&diffuseTexture2DDesc, &diffuseTexture2DData, &diffuseTex);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Diffuse Texture2D \n";
	}

	this->_diffuseTexture2D = diffuseTex;
	
	ID3D11ShaderResourceView* diffuseTexSRV;
	device->CreateShaderResourceView(diffuseTex, nullptr, &diffuseTexSRV);
	
	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Diffuse SRV \n";
	}
	
	this->_diffuseTextureSRV = diffuseTexSRV;
	
	// SPECULAR 
	ImageData specularData = subMeshInfo.MTLData.specularTextureData;
				
	D3D11_TEXTURE2D_DESC specularTexture2DDesc;
	specularTexture2DDesc.Width = static_cast<UINT>(specularData.Width);
	specularTexture2DDesc.Height = static_cast<UINT>(specularData.Height);
	specularTexture2DDesc.MipLevels = 1;
	specularTexture2DDesc.ArraySize = 1;
	specularTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	specularTexture2DDesc.SampleDesc = DXGI_SAMPLE_DESC{1, 0};
	specularTexture2DDesc.Usage = D3D11_USAGE_IMMUTABLE;
	specularTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	specularTexture2DDesc.CPUAccessFlags = 0;
	specularTexture2DDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA specularTexture2DData;
	specularTexture2DData.pSysMem = specularData.TextureData.data(); 
	specularTexture2DData.SysMemPitch = static_cast<UINT>(specularData.Width) * specularData.RGBAChannels;
	specularTexture2DData.SysMemSlicePitch = 0;

	ID3D11Texture2D* specularTex;
	hr = device->CreateTexture2D(&specularTexture2DDesc, &specularTexture2DData, &specularTex);
	
	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Specular Texture2D \n";
	}

	this->_specularTexture2D = specularTex;
	
	ID3D11ShaderResourceView* specularTexSRV;           	
	device->CreateShaderResourceView(specularTex, nullptr, &specularTexSRV);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Specular SRV \n";
	}

	this->_specularTextureSRV = specularTexSRV;
	
	// COLOUR 
	ImageData colourData = subMeshInfo.MTLData.colourTextureData;
				
	D3D11_TEXTURE2D_DESC colourTexture2DDesc;
	colourTexture2DDesc.Width = static_cast<UINT>(colourData.Width);
	colourTexture2DDesc.Height = static_cast<UINT>(colourData.Height);
	colourTexture2DDesc.MipLevels = 1;
	colourTexture2DDesc.ArraySize = 1;
	colourTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	colourTexture2DDesc.SampleDesc = DXGI_SAMPLE_DESC{1, 0};
	colourTexture2DDesc.Usage = D3D11_USAGE_IMMUTABLE;
	colourTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	colourTexture2DDesc.CPUAccessFlags = 0;
	colourTexture2DDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA colourTexture2DData;
	colourTexture2DData.pSysMem = colourData.TextureData.data(); 
	colourTexture2DData.SysMemPitch = static_cast<UINT>(colourData.Width) * colourData.RGBAChannels;
	colourTexture2DData.SysMemSlicePitch = 0;

	ID3D11Texture2D* colourTex;
	hr = device->CreateTexture2D(&colourTexture2DDesc, &colourTexture2DData, &colourTex);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Colour Texture2D \n";
	}

	this->_colourTexture2D = colourTex;

	ID3D11ShaderResourceView* colourTexSRV;
	device->CreateShaderResourceView(colourTex, nullptr, &colourTexSRV);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Colour SRV \n";
	}

	this->_colourTextureSRV = colourTexSRV;
}

size_t SubMesh::GetNumIndices()
{
	return this->_nrOfIndices;
}

size_t SubMesh::GetStartIndex()
{
	return this->_startIndex;
}

ID3D11ShaderResourceView *SubMesh::GetAmbientSRV() const
{
	return this->_ambientTextureSRV.GetSRV();
}

ID3D11ShaderResourceView *SubMesh::GetDiffuseSRV() const
{
	return this->_diffuseTextureSRV.GetSRV();
}

ID3D11ShaderResourceView *SubMesh::GetSpecularSRV() const
{
	return this->_specularTextureSRV.GetSRV();
}

ID3D11ShaderResourceView *SubMesh::GetColourTextureSRV() const
{
	return this->_colourTextureSRV.GetSRV();
}

float SubMesh::GetSpecularExponent() const
{
	return this->_specularExponent;
}