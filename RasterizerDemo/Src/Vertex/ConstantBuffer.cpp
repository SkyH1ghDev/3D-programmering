#include "ConstantBuffer.hpp"

#include <iostream>

ConstantBuffer::ConstantBuffer(HRESULT &hr, ID3D11Device *device, size_t byteSize,
                               void *initialData = nullptr,
                               unsigned int sysMemPitch = 0,
                               unsigned int sysMemSlicePitch = 0,
                               D3D11_USAGE usageFlag = D3D11_USAGE_IMMUTABLE, 
                               D3D11_CPU_ACCESS_FLAG cpuAccessFlag = static_cast<D3D11_CPU_ACCESS_FLAG>(0),
                               D3D11_RESOURCE_MISC_FLAG miscFlags = static_cast<D3D11_RESOURCE_MISC_FLAG>(0),
                               unsigned int structureByStride = 0)
{
	static_assert(byteSize % 16 == 0, "Size of constant buffer is not divisible by 16");
	
	D3D11_BUFFER_DESC constBufferDesc;
 	constBufferDesc.ByteWidth = sizeof(byteSize);
 	constBufferDesc.Usage = usageFlag;
 	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 	constBufferDesc.CPUAccessFlags = cpuAccessFlag;
 	constBufferDesc.MiscFlags = miscFlags;
 	constBufferDesc.StructureByteStride = structureByStride;
 
 	D3D11_SUBRESOURCE_DATA constBufferSubResource;
 	constBufferSubResource.pSysMem = &initialData;
 	constBufferSubResource.SysMemPitch = sysMemPitch;
 	constBufferSubResource.SysMemSlicePitch = sysMemSlicePitch;

	ID3D11Buffer* constBuffer;
	hr = device->CreateBuffer(&constBufferDesc, &constBufferSubResource, &constBuffer);

	if(!FAILED(hr))
	{
	    this->_buffer = std::unique_ptr<ID3D11Buffer>(constBuffer);
		this->_bufferSize = byteSize;
	}

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

ID3D11Buffer *ConstantBuffer::GetBuffer() const
{
   return this->_buffer.get();
}

size_t ConstantBuffer::GetSize() const
{
   return this->_bufferSize;
}

ConstantBuffer::~ConstantBuffer()
{
   this->_buffer->Release();
}
