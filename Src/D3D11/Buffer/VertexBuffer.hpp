#pragma once

#include "Vertex.hpp"
#include <d3d11_4.h>
#include <vector>

class VertexBuffer
{
public:
	VertexBuffer() = default;
	VertexBuffer(HRESULT& hr, ID3D11Device* device, UINT sizeOfVertex, 
		UINT nrOfVerticesInBuffer, const std::vector<Vertex>& vertexData);
	~VertexBuffer();
	VertexBuffer(const VertexBuffer& other);
	VertexBuffer& operator=(const VertexBuffer& other);
	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void Initialize(ID3D11Device* device, UINT sizeOfVertex,
		UINT nrOfVerticesInBuffer, void* vertexData);

	UINT GetNrOfVertices() const;
	UINT GetVertexSize() const;
	ID3D11Buffer* GetBuffer() const;
	
private:
	ID3D11Buffer* _buffer = nullptr;
	UINT _nrOfVertices = 0;
	UINT _vertexSize = 0;
};