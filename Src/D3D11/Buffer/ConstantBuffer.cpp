#include "ConstantBuffer.hpp"

#include <iostream>

ConstantBuffer::ConstantBuffer(HRESULT &hr, ID3D11Device *device, size_t byteSize,
                               void *initialData,
                               unsigned int sysMemPitch,
                               unsigned int sysMemSlicePitch,
                               unsigned int structureByStride,
                               BufferDescData flags)
{
	if (byteSize % 16 != 0)
	{
		std::cerr << "Size of constant buffer is not divisible by 16" << "\n";
		throw std::runtime_error("");
	}
	
	D3D11_BUFFER_DESC constBufferDesc;
 	constBufferDesc.ByteWidth = byteSize;
 	constBufferDesc.Usage = flags.Usage;
 	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 	constBufferDesc.CPUAccessFlags = flags.CpuAccess;
 	constBufferDesc.MiscFlags = flags.Misc;
 	constBufferDesc.StructureByteStride = structureByStride;
 
 	D3D11_SUBRESOURCE_DATA constBufferSubResource;
 	constBufferSubResource.pSysMem = initialData;
 	constBufferSubResource.SysMemPitch = sysMemPitch;
 	constBufferSubResource.SysMemSlicePitch = sysMemSlicePitch;

	ID3D11Buffer* constBuffer;
	hr = device->CreateBuffer(&constBufferDesc, &constBufferSubResource, &constBuffer);

	if(FAILED(hr))
	{
		throw std::runtime_error("Failed to create Constant Buffer");	
	}
	
	this->_buffer = constBuffer;
	this->_bufferSize = byteSize;


	
}

ConstantBuffer::ConstantBuffer(ConstantBuffer &&other) noexcept
{
	std::swap(this->_buffer, other._buffer);
	std::swap(this->_bufferSize, other._bufferSize);	
}

ConstantBuffer &ConstantBuffer::operator=(ConstantBuffer &&other) noexcept
{
	std::swap(this->_buffer, other._buffer);
	std::swap(this->_bufferSize, other._bufferSize);	

	return *this;
}

void ConstantBuffer::UpdateBuffer(ID3D11DeviceContext *context, void *data, const size_t &byteSize)
{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(mappedResource));

		context->Map(this->_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, data, byteSize); 
		context->Unmap(this->_buffer, 0);
}


ID3D11Buffer *ConstantBuffer::GetBuffer() const
{
   return this->_buffer;
}

size_t ConstantBuffer::GetSize() const
{
   return this->_bufferSize;
}

ConstantBuffer::~ConstantBuffer()
{
	if (this->_buffer != nullptr)
	{
		this->_buffer->Release();
	}
}

ConstantBuffer::ConstantBuffer(const ConstantBuffer &other)
{
	this->_buffer = other._buffer; this->_buffer->AddRef();
	this->_bufferSize = other._bufferSize;
}

ConstantBuffer& ConstantBuffer::operator=(const ConstantBuffer& other)
{
	if (this == &other)
	{
		return *this;
	}
	
	this->_buffer = other._buffer; this->_buffer->AddRef();
	this->_bufferSize = other._bufferSize;

	return *this;
}
