#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(ID3D11Device *device, UINT sizeOfVertex, UINT nrOfVerticesInBuffer, void *vertexData)
{
    this->_vertexSize = sizeOfVertex;
    this->_nrOfVertices = nrOfVerticesInBuffer;
    this->_buffer = device->CreateBuffer()
}
