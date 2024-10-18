#pragma once

#include <d3d11_4.h>

#include "RenderTargetView.hpp"
#include "ShaderResourceView.hpp"
#include "Texture2D.hpp"

class RenderBuffer
{
public:
	RenderBuffer() = delete;
	~RenderBuffer() = default;
	RenderBuffer(ID3D11RenderTargetView* rtv, ID3D11Texture2D* texture, ID3D11ShaderResourceView* srv);
	RenderBuffer(ID3D11RenderTargetView* rtv);
	RenderBuffer(const RenderBuffer& other) = default;
	RenderBuffer& operator=(const RenderBuffer& other) = default;
	RenderBuffer(RenderBuffer&& other) noexcept = default;
	RenderBuffer& operator=(RenderBuffer&& other) noexcept = default;

	ID3D11RenderTargetView* GetRTV() const;
	ID3D11ShaderResourceView* GetSRV() const;
	
private:
	Texture2D _texture; 
	RenderTargetView _rtv;
	ShaderResourceView _srv;
};
