#pragma once

#include <d3d11_4.h>

class RenderTarget
{
public:
	RenderTarget() = delete;
	~RenderTarget();
	RenderTarget(ID3D11Device* device, IDXGISwapChain* swapChain);
	RenderTarget(const RenderTarget& other) = delete;
	RenderTarget& operator=(const RenderTarget& other) = delete;
	RenderTarget(RenderTarget&& other) = delete;
	RenderTarget& operator=(RenderTarget&& other) noexcept;

	void Initialize(ID3D11Device* device, UINT width, UINT height,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, bool hasSRV = false);

	ID3D11RenderTargetView* GetRTV() const;
	ID3D11ShaderResourceView* GetSRV() const;
	
private:
	ID3D11Texture2D* _texture = nullptr; // Deferred rendering. Render lighting information to this for example.
	ID3D11RenderTargetView* _rtv = nullptr;
	ID3D11ShaderResourceView* _srv = nullptr;
};