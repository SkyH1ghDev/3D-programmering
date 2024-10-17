#include "StructuredBuffer.hpp"

#include <iostream>

StructuredBuffer::StructuredBuffer(HRESULT& hr, ID3D11Device* device,
								   UINT elementSize,
								   UINT nrOfElements,
                                   void* initialData,
                                   unsigned sysMemPitch,
                                   unsigned sysMemSlicePitch)
{
	if (elementSize * nrOfElements % 16 != 0)
	{
		std::cerr << "Size of structured buffer is not divisible by 16";
		throw std::runtime_error("");
	}
    
	D3D11_BUFFER_DESC structuredBufferDesc;
	structuredBufferDesc.ByteWidth = elementSize * nrOfElements;
	structuredBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	structuredBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	structuredBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	structuredBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	structuredBufferDesc.StructureByteStride = elementSize;
 
	D3D11_SUBRESOURCE_DATA structuredBufferSubResource;
	structuredBufferSubResource.pSysMem = initialData;
	structuredBufferSubResource.SysMemPitch = sysMemPitch;
	structuredBufferSubResource.SysMemSlicePitch = sysMemSlicePitch;

	ID3D11Buffer* structuredBuffer;
	hr = device->CreateBuffer(&structuredBufferDesc, &structuredBufferSubResource, &structuredBuffer);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create Structured Buffer");
	}
	
	this->_buffer = structuredBuffer;
	this->_elementSize = elementSize;
	this->_nrOfElements = nrOfElements;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = this->_nrOfElements;

	ID3D11ShaderResourceView* srv;
	hr = device->CreateShaderResourceView(this->_buffer, &srvDesc, &srv);
	
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to Create Structured Buffer SRV");
	}

	this->_srv = srv;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = this->_nrOfElements;
	uavDesc.Buffer.Flags = 0;

	ID3D11UnorderedAccessView* uav;
	hr = device->CreateUnorderedAccessView(this->_buffer, &uavDesc, &uav);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to Create Structured Buffer UAV");
	}

	this->_uav = uav;
}

StructuredBuffer::StructuredBuffer(StructuredBuffer&& other) noexcept
{
	std::swap(this->_buffer, other._buffer);
	std::swap(this->_srv, other._srv);
	std::swap(this->_uav, other._uav);
	std::swap(this->_elementSize, other._elementSize);
	std::swap(this->_nrOfElements, other._nrOfElements);
}

StructuredBuffer& StructuredBuffer::operator=(StructuredBuffer&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	
	std::swap(this->_buffer, other._buffer);
	std::swap(this->_srv, other._srv);
	std::swap(this->_uav, other._uav);
	std::swap(this->_elementSize, other._elementSize);
	std::swap(this->_nrOfElements, other._nrOfElements);

	return *this;
}

void StructuredBuffer::UpdateBuffer(ID3D11DeviceContext* context, void* data, UINT elementSize, UINT nrOfElements) const
{
	
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(mappedResource));

		context->Map(this->_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, data, elementSize * nrOfElements); 
		context->Unmap(this->_buffer, 0);
}

ID3D11Buffer* StructuredBuffer::GetBuffer() const
{
	return this->_buffer;
}

UINT StructuredBuffer::GetElementSize() const
{
	return this->_elementSize;
}

UINT StructuredBuffer::GetNrOfElements() const
{
	return this->_nrOfElements;
}

ID3D11ShaderResourceView* StructuredBuffer::GetSRV() const
{
	return this->_srv.GetSRV();
}

ID3D11UnorderedAccessView* StructuredBuffer::GetUAV() const
{
	return this->_uav.GetUAV();
}

StructuredBuffer::~StructuredBuffer()
{
	if (this->_buffer != nullptr)
	{
		this->_buffer->Release();
	}
}

StructuredBuffer::StructuredBuffer(const StructuredBuffer& other)
{
	this->_buffer = other._buffer; this->_buffer->AddRef();
	this->_elementSize = other._elementSize;
	this->_nrOfElements = other._nrOfElements;
	this->_srv = other._srv; 
	this->_uav = other._uav;
}

StructuredBuffer& StructuredBuffer::operator=(const StructuredBuffer& other)
{
	if (this == &other)
	{
		return *this;
	}

	this->_buffer = other._buffer; this->_buffer->AddRef();
	this->_elementSize = other._elementSize;
	this->_nrOfElements = other._nrOfElements;
	this->_srv = other._srv;
	this->_uav = other._uav;

	return *this;
}


