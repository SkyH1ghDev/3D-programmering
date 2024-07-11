#pragma once

#include <d3d11_4.h>

class RenderTargetView
{
public:
	RenderTargetView() = delete;
	~RenderTargetView();
	RenderTargetView(ID3D11RenderTargetView* rtv, ID3D11Texture2D* texture, ID3D11ShaderResourceView* srv);
	RenderTargetView(ID3D11RenderTargetView*);
	RenderTargetView(const RenderTargetView& other);
	RenderTargetView& operator=(const RenderTargetView& other);
	RenderTargetView(RenderTargetView&& other) noexcept;
	RenderTargetView& operator=(RenderTargetView&& other) noexcept;

	void Initialize(ID3D11Device* device, UINT width, UINT height,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, bool hasSRV = false);

	ID3D11RenderTargetView* GetRTV() const;
	ID3D11ShaderResourceView* GetSRV() const;
	
private:
	ID3D11Texture2D* _texture = nullptr; 
	ID3D11RenderTargetView* _rtv = nullptr;
	ID3D11ShaderResourceView* _srv = nullptr;
};