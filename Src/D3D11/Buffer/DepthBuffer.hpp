#pragma once

#include <d3d11_4.h>

#include "DepthStencilView.hpp"
#include "Texture2D.hpp"
#include "ShaderResourceView.hpp"

class DepthBuffer
{
public:
	DepthBuffer() = delete;
	DepthBuffer(HRESULT& hr, ID3D11Device* device);
	DepthBuffer(HRESULT& hr, ID3D11Device* device, const UINT& width, const UINT& height);
	~DepthBuffer() = default;
	DepthBuffer(const DepthBuffer& other) = default;
	DepthBuffer& operator=(const DepthBuffer& other) = default;
	DepthBuffer(DepthBuffer&& other) noexcept = default;
	DepthBuffer& operator=(DepthBuffer&& other) noexcept = default;

	void Initialize(ID3D11Device* device, UINT width, UINT height,
		bool hasSRV = false, UINT arraySize = 1);

	ID3D11DepthStencilView* GetDSV() const;
	ID3D11ShaderResourceView* GetSRV() const;
	
private:
	Texture2D _texture;
	DepthStencilView _dsv;
	ShaderResourceView _srv;
};
