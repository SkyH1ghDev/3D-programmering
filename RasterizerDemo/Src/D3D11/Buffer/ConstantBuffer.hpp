#pragma once

#include <d3d11_4.h>
#include "BufferFlagData.hpp"

class ConstantBuffer
{
private:
	ID3D11Buffer* _buffer = nullptr;
	size_t _bufferSize = 0;

public:
	ConstantBuffer() = delete;
	ConstantBuffer(HRESULT &hr, ID3D11Device *device, size_t byteSize, void *initialData, unsigned sysMemPitch, unsigned sysMemSlicePitch, unsigned structureByStride, BufferFlagData flags);
	~ConstantBuffer();
	ConstantBuffer(const ConstantBuffer& other) = delete;
	ConstantBuffer& operator=(const ConstantBuffer& other) = delete;
	ConstantBuffer(ConstantBuffer&& other) noexcept; // Move constructor
	ConstantBuffer& operator=(ConstantBuffer&& other) noexcept; // Move assignment operator

	void Initialize(ID3D11Device* device, size_t byteSize, void* initialData = nullptr);

	size_t GetSize() const;
	ID3D11Buffer* GetBuffer() const;

	void UpdateBuffer(ID3D11DeviceContext* context, void* data, const size_t& byteSize);
};