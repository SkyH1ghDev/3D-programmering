#pragma once

#include <d3d11_4.h>

class SubMesh
{
public:
	SubMesh() = delete;
	SubMesh(size_t startIndexValue, size_t nrOfIndicesInSubMesh, float specularExponent,
		ID3D11ShaderResourceView* ambientTextureSRV, ID3D11Texture2D* ambientTexture2D,
		ID3D11ShaderResourceView* diffuseTextureSRV, ID3D11Texture2D* diffuseTexture2D,
		ID3D11ShaderResourceView* specularTextureSRV, ID3D11Texture2D* specularTexture2D);
	~SubMesh();
	SubMesh(const SubMesh& other) = default;
	SubMesh& operator=(const SubMesh& other) = default;
	SubMesh(SubMesh&& other) = default;
	SubMesh& operator=(SubMesh&& other) = default;

	void Initialize(size_t startIndexValue, size_t nrOfIndicesInSubMesh,
		ID3D11ShaderResourceView* ambientTextureSRV, ID3D11ShaderResourceView* diffuseTextureSRV,
		ID3D11ShaderResourceView* specularTextureSRV);

	void PerformDrawCall(ID3D11DeviceContext* context) const;

	ID3D11ShaderResourceView* GetAmbientSRV() const;
	ID3D11ShaderResourceView* GetDiffuseSRV() const;
	ID3D11ShaderResourceView* GetSpecularSRV() const;
	
private:
	size_t _startIndex = 0;
	size_t _nrOfIndices = 0;
	float _specularExponent = 0.0f;

	ID3D11ShaderResourceView* _ambientTextureSRV = nullptr;
	ID3D11Texture2D* _ambientTexture2D = nullptr;
	
	ID3D11ShaderResourceView* _diffuseTextureSRV = nullptr;
	ID3D11Texture2D* _diffuseTexture2D = nullptr;
	
	ID3D11ShaderResourceView* _specularTextureSRV = nullptr;
	ID3D11Texture2D* _specularTexture2D = nullptr;
};