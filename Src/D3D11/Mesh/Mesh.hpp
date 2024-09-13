#pragma once

#include <vector>
#include <array>
#include <bitset>

#include <d3d11_4.h>

#include "SubMesh.hpp"
#include "VertexBuffer.hpp"
#include "MeshData.hpp"

class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;
	Mesh(HRESULT& hr, ID3D11Device* device, const MeshData& meshData);
	Mesh(const Mesh & other) = default;
	Mesh& operator=(const Mesh & other) = default;
	Mesh(Mesh && other) = default;
	Mesh& operator=(Mesh && other) = default;

	void Initialize(ID3D11Device* device, const MeshData& meshInfo);

	void BindMeshBuffers(ID3D11DeviceContext* context) const;
	void PerformSubMeshDrawCall(ID3D11DeviceContext* context, size_t subMeshIndex) const;
	void PerformOscillation(float deltaTime); // Only oscillating meshes

	bool IsOscillating() const;
	
	SubMesh& GetSubMeshAt(size_t index);
	size_t GetNrOfSubMeshes() const;
	VertexBuffer GetVertexBuffer();
	std::array<float, 4> GetCurrentPosition() const;
	ID3D11ShaderResourceView* GetAmbientSRV(size_t subMeshIndex) const;
	ID3D11ShaderResourceView* GetDiffuseSRV(size_t subMeshIndex) const;
	ID3D11ShaderResourceView* GetSpecularSRV(size_t subMeshIndex) const;
	ID3D11ShaderResourceView* GetTextureSRV(size_t subMeshIndex) const;
	
private:
	std::vector<SubMesh> _subMeshes;
	VertexBuffer _vertexBuffer;

	std::array<float, 4> _currentPosition = {0.0f, 0.0f, 0.0f, 1.0f};

	std::array<float, 4> _oscillationEndPosition;
	std::array<float, 4> _oscillationStartPosition;
	float _oscillationPeriod;
	float _oscillationTime;
	
	
	std::bitset<1> _meshFlags;
};