#pragma once

#include <vector>

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

	size_t GetNrOfSubMeshes() const;
	ID3D11ShaderResourceView* GetAmbientSRV(size_t subMeshIndex) const;
	ID3D11ShaderResourceView* GetDiffuseSRV(size_t subMeshIndex) const;
	ID3D11ShaderResourceView* GetSpecularSRV(size_t subMeshIndex) const;
	
private:
	std::vector<SubMesh> _subMeshes;
	VertexBuffer _vertexBuffer = VertexBuffer();
	
	// IndexBuffer is unused as the current implementation uses the Triangle List topology rather than indexed
	// IndexBuffer _indexBuffer;
};