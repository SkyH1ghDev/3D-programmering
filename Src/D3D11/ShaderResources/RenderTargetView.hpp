#pragma once

#include <d3d11.h>

class RenderTargetView
{
public:
    RenderTargetView() = default;
    RenderTargetView(ID3D11RenderTargetView* rtv);
    RenderTargetView(const RenderTargetView& other);
    RenderTargetView& operator=(const RenderTargetView& other);
    RenderTargetView(RenderTargetView&& other) noexcept;
    RenderTargetView& operator=(RenderTargetView&& other) noexcept;

    ~RenderTargetView();

public:
    ID3D11RenderTargetView* GetRTV() const;

private:
    ID3D11RenderTargetView* _rtv = nullptr;
};
