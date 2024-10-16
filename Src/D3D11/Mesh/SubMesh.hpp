#pragma once

#include <d3d11_4.h>

#include "ShaderResourceView.hpp"
#include "SubMeshInfo.hpp"
#include "Texture2D.hpp"

class SubMesh
{
public:
	SubMesh() = delete;
	SubMesh(HRESULT& hr, ID3D11Device* device, const SubMeshInfo& subMeshInfo);

	ID3D11ShaderResourceView* GetAmbientSRV() const;
	ID3D11ShaderResourceView* GetDiffuseSRV() const;
	ID3D11ShaderResourceView* GetSpecularSRV() const;
	ID3D11ShaderResourceView* GetColourTextureSRV() const;
	float GetSpecularExponent() const;

	size_t GetStartIndex();
	size_t GetNumIndices();
	
private:
	size_t _startIndex = 0;
	size_t _nrOfIndices = 0;
	float _specularExponent = 10000.0f;

	ShaderResourceView _ambientTextureSRV;
	Texture2D _ambientTexture2D;
	
	ShaderResourceView _diffuseTextureSRV;
	Texture2D _diffuseTexture2D;
	
	ShaderResourceView _specularTextureSRV;
	Texture2D _specularTexture2D;

	ShaderResourceView _colourTextureSRV;
	Texture2D _colourTexture2D;
};
