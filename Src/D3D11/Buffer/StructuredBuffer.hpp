#pragma once

#include <d3d11_4.h>
#include <BufferDescData.hpp>

#include "ShaderResourceView.hpp"
#include "UnorderedAccessView.hpp"

class StructuredBuffer
{
public:
	StructuredBuffer() = default;
	StructuredBuffer(HRESULT &hr, ID3D11Device *device, UINT elementSize, UINT nrOfElements, void *initialData, unsigned sysMemPitch, unsigned sysMemSlicePitch, unsigned structureByStride, BufferDescData flags);
	~StructuredBuffer();
	StructuredBuffer(const StructuredBuffer& other);
	StructuredBuffer& operator=(const StructuredBuffer& other);
	StructuredBuffer(StructuredBuffer&& other) noexcept; // Move constructor
	StructuredBuffer& operator=(StructuredBuffer&& other) noexcept; // Move assignment operator

	void UpdateBuffer(ID3D11DeviceContext* context, void* data, UINT elementSize, UINT nrOfElements) const;

	ID3D11Buffer* GetBuffer() const;
	UINT GetElementSize() const;
	UINT GetNrOfElements() const;
	ID3D11ShaderResourceView* GetSRV() const;
	ID3D11UnorderedAccessView* GetUAV() const;
	
private:
	ID3D11Buffer* _buffer = nullptr;
	ShaderResourceView _srv;
	UnorderedAccessView _uav;
	UINT _elementSize = 0;
	UINT _nrOfElements = 0;
};
