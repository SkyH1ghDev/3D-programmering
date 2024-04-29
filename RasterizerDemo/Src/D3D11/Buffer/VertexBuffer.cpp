#include "VertexBuffer.hpp"

#include <d3d11_4.h>

VertexBuffer::VertexBuffer(ID3D11Device *device, UINT sizeOfVertex, UINT nrOfVerticesInBuffer, void *vertexData)
{
    this->_vertexSize = sizeOfVertex;
    this->_nrOfVertices = nrOfVerticesInBuffer;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeOfVertex * nrOfVerticesInBuffer;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	device->CreateBuffer(&bufferDesc, &data, &this->_buffer);
}

ID3D11Buffer* VertexBuffer::GetBuffer() const
{
	return this->_buffer;
}

VertexBuffer::~VertexBuffer()
{
	this->_buffer->Release();
}


