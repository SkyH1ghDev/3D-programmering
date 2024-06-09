#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(HRESULT& hr, ID3D11Device *device, UINT sizeOfVertex, UINT nrOfVerticesInBuffer, const std::vector<Vertex>& vertexData)
{
    this->_vertexSize = sizeOfVertex;
    this->_nrOfVertices = nrOfVerticesInBuffer;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = this->_vertexSize * this->_nrOfVertices;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexData.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	
    hr = device->CreateBuffer(&bufferDesc, &data, &this->_buffer);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	this->_buffer = std::move(other.GetBuffer());
	this->_vertexSize = std::move(other.GetVertexSize());
	this->_nrOfVertices = std::move(other.GetNrOfVertices());
	
	return *this;
}

VertexBuffer::~VertexBuffer()
{
    this->_buffer->Release();
}

ID3D11Buffer* VertexBuffer::GetBuffer() const
{
	return this->_buffer;
}

UINT VertexBuffer::GetVertexSize() const
{
	return this->_vertexSize;
}

UINT VertexBuffer::GetNrOfVertices() const
{
	return this->_nrOfVertices;
}

