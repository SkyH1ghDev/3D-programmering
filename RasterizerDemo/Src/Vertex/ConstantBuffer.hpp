#pragma once

#include <d3d11_4.h>
#include <memory>

class ConstantBufferFlagData;

class ConstantBuffer
{
private:
	std::unique_ptr<ID3D11Buffer> _buffer = nullptr;
	size_t _bufferSize = 0;

public:
	ConstantBuffer() = delete;
	ConstantBuffer(HRESULT &hr, ID3D11Device *device, size_t byteSize, void *initialData, unsigned sysMemPitch, unsigned sysMemSlicePitch, D3D11_USAGE usageFlag, D3D11_CPU_ACCESS_FLAG cpuAccessFlag, D3D11_RESOURCE_MISC_FLAG miscFlags, unsigned structureByStride);
	~ConstantBuffer();
	ConstantBuffer(const ConstantBuffer& other) = delete;
	ConstantBuffer& operator=(const ConstantBuffer& other) = delete;
	ConstantBuffer(ConstantBuffer&& other) noexcept; // Move constructor
	ConstantBuffer& operator=(ConstantBuffer&& other) noexcept; // Move assignment operator

	void Initialize(ID3D11Device* device, size_t byteSize, void* initialData = nullptr);

	size_t GetSize() const;
	ID3D11Buffer* GetBuffer() const;

	void UpdateBuffer(ID3D11DeviceContext* context, void* data);
};