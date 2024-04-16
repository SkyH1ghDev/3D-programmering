#include "ConstantBuffer.hpp"

#include <iostream>

ConstantBuffer::ConstantBuffer(HRESULT &hr, ID3D11Device *device, size_t byteSize,
                               void *initialData,
                               unsigned int sysMemPitch,
                               unsigned int sysMemSlicePitch,
                               unsigned int structureByStride,
                               BufferFlagData flags)
{
	if (byteSize % 16 != 0)
	{
		std::cerr << "Size of constant buffer is not divisible by 16" << "\n";
		hr = -1;
	}
	
	D3D11_BUFFER_DESC constBufferDesc;
 	constBufferDesc.ByteWidth = byteSize;
 	constBufferDesc.Usage = flags.Usage;
 	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
 	constBufferDesc.CPUAccessFlags = flags.CpuAccess;
 	constBufferDesc.MiscFlags = flags.Misc;
 	constBufferDesc.StructureByteStride = structureByStride;
 
 	D3D11_SUBRESOURCE_DATA constBufferSubResource;
 	constBufferSubResource.pSysMem = &initialData;
 	constBufferSubResource.SysMemPitch = sysMemPitch;
 	constBufferSubResource.SysMemSlicePitch = sysMemSlicePitch;

	ID3D11Buffer* constBuffer;
	hr = device->CreateBuffer(&constBufferDesc, &constBufferSubResource, &constBuffer);

	if(!FAILED(hr))
	{
		this->_buffer = constBuffer;
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
		std::cout << this->GetSize() << "\n";
		this->_buffer->Release();
	}
}
