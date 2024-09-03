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

	hr = device->CreateTexture2D(&ambientTexture2DDesc, &ambientTexture2DData, &this->_ambientTexture2D);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Ambient Texture2D \n";
	}
	
	device->CreateShaderResourceView(this->_ambientTexture2D, nullptr, &this->_ambientTextureSRV);
	
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

	hr = device->CreateTexture2D(&diffuseTexture2DDesc, &diffuseTexture2DData, &this->_diffuseTexture2D);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Diffuse Texture2D \n";
	}
	
	device->CreateShaderResourceView(this->_ambientTexture2D, nullptr, &this->_diffuseTextureSRV);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Diffuse SRV \n";
	}
	
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

	hr = device->CreateTexture2D(&specularTexture2DDesc, &specularTexture2DData, &this->_specularTexture2D);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Specular Texture2D \n";
	}
            	
	device->CreateShaderResourceView(this->_specularTexture2D, nullptr, &this->_specularTextureSRV);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Specular SRV \n";
	}
	
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

	hr = device->CreateTexture2D(&colourTexture2DDesc, &colourTexture2DData, &this->_colourTexture2D);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Colour Texture2D \n";
	}
            	
	device->CreateShaderResourceView(this->_colourTexture2D, nullptr, &this->_colourTextureSRV);

	if (FAILED(hr))
	{
		std::cerr << "Failed To Create Colour SRV \n";
	}
}

SubMesh::SubMesh(SubMesh&& other) noexcept
{
	this->_specularExponent = other._specularExponent;
	this->_startIndex = other._startIndex;
	this->_nrOfIndices = other._nrOfIndices;
	
	this->_specularTexture2D = other._specularTexture2D; this->_specularTexture2D->AddRef();
	this->_specularTextureSRV = other._specularTextureSRV; this->_specularTextureSRV->AddRef();
	
	this->_ambientTexture2D= other._ambientTexture2D; this->_ambientTexture2D->AddRef();
	this->_ambientTextureSRV = other._ambientTextureSRV; this->_ambientTextureSRV->AddRef();
	
	this->_diffuseTexture2D = other._diffuseTexture2D; this->_diffuseTexture2D->AddRef();
	this->_diffuseTextureSRV = other._diffuseTextureSRV; this->_diffuseTextureSRV->AddRef();

	this->_colourTexture2D = other._colourTexture2D; this->_colourTexture2D->AddRef();
	this->_colourTextureSRV = other._colourTextureSRV; this->_colourTextureSRV->AddRef();
}

SubMesh::~SubMesh()
{
    this->_specularTextureSRV->Release(); this->_specularTextureSRV = nullptr;
    this->_specularTexture2D->Release(); this->_specularTexture2D = nullptr;

    this->_diffuseTextureSRV->Release(); this->_diffuseTextureSRV = nullptr;
    this->_diffuseTexture2D->Release(); this->_diffuseTexture2D = nullptr;
    
    this->_ambientTextureSRV->Release(); this->_ambientTextureSRV = nullptr;
    this->_ambientTexture2D->Release(); this->_ambientTexture2D = nullptr;

    this->_colourTextureSRV->Release(); this->_colourTextureSRV = nullptr;
    this->_colourTexture2D->Release(); this->_colourTexture2D = nullptr;
}
SubMesh::SubMesh(const SubMesh &other)
{
	this->_specularExponent = other._specularExponent;
	this->_startIndex = other._startIndex;
	this->_nrOfIndices = other._nrOfIndices;
	
	this->_specularTexture2D = other._specularTexture2D; this->_specularTexture2D->AddRef();
	this->_specularTextureSRV = other._specularTextureSRV; this->_specularTextureSRV->AddRef();
	
	this->_ambientTexture2D= other._ambientTexture2D; this->_ambientTexture2D->AddRef();
	this->_ambientTextureSRV = other._ambientTextureSRV; this->_ambientTextureSRV->AddRef();
	
	this->_diffuseTexture2D = other._diffuseTexture2D; this->_diffuseTexture2D->AddRef();
	this->_diffuseTextureSRV = other._diffuseTextureSRV; this->_diffuseTextureSRV->AddRef();

	this->_colourTexture2D = other._colourTexture2D; this->_colourTexture2D->AddRef();
	this->_colourTextureSRV = other._colourTextureSRV; this->_colourTextureSRV->AddRef();
}

SubMesh & SubMesh::operator=(const SubMesh &other)
{
	if (this == &other)
	{
		return *this;
	}
	
	this->_specularExponent = other._specularExponent;
	this->_startIndex = other._startIndex;
	this->_nrOfIndices = other._nrOfIndices;
	
	this->_specularTexture2D = other._specularTexture2D; this->_specularTexture2D->AddRef();
	this->_specularTextureSRV = other._specularTextureSRV; this->_specularTextureSRV->AddRef();
	
	this->_ambientTexture2D= other._ambientTexture2D; this->_ambientTexture2D->AddRef();
	this->_ambientTextureSRV = other._ambientTextureSRV; this->_ambientTextureSRV->AddRef();
	
	this->_diffuseTexture2D = other._diffuseTexture2D; this->_diffuseTexture2D->AddRef();
	this->_diffuseTextureSRV = other._diffuseTextureSRV; this->_diffuseTextureSRV->AddRef();

	this->_colourTexture2D = other._colourTexture2D; this->_colourTexture2D->AddRef();
	this->_colourTextureSRV = other._colourTextureSRV; this->_colourTextureSRV->AddRef();
	
	return *this;
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
	return this->_ambientTextureSRV;
}

ID3D11ShaderResourceView *SubMesh::GetDiffuseSRV() const
{
	return this->_diffuseTextureSRV;
}

ID3D11ShaderResourceView *SubMesh::GetSpecularSRV() const
{
	return this->_specularTextureSRV;
}

ID3D11ShaderResourceView *SubMesh::GetColourTextureSRV() const
{
	return this->_colourTextureSRV;
}


void SubMesh::PerformDrawCall(ID3D11DeviceContext *context) const
{
	
}
