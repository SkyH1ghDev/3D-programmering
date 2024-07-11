#pragma once

#include <d3d11_4.h>

class DepthBuffer
{
public:
	DepthBuffer() = delete;
	DepthBuffer(HRESULT& hr, ID3D11Device* device);
	~DepthBuffer();
	DepthBuffer(const DepthBuffer& other);
	DepthBuffer& operator=(const DepthBuffer& other);
	DepthBuffer(DepthBuffer&& other) noexcept;
	DepthBuffer& operator=(DepthBuffer&& other) noexcept;

	void Initialize(ID3D11Device* device, UINT width, UINT height,
		bool hasSRV = false, UINT arraySize = 1);

	ID3D11DepthStencilView* GetDSV() const;
	ID3D11ShaderResourceView* GetSRV() const;
	
private:
	ID3D11Texture2D* _texture = nullptr;
	ID3D11DepthStencilView* _dsv = nullptr;
	ID3D11ShaderResourceView* _srv = nullptr;
};
