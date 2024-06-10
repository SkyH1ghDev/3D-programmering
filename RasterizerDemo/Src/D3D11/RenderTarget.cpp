#include "RenderTarget.hpp"

#include "D3D11Helper.hpp"

// TODO: Refactor RenderTarget(device, swapChain) to SetupHelper instead.

RenderTarget::RenderTarget(ID3D11Device *device, IDXGISwapChain *swapChain)
{
    D3D11Helper d3d11Helper;

    d3d11Helper.CreateRenderTargetView(device, swapChain, this->_rtv);
}

RenderTarget& RenderTarget::operator=(RenderTarget&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    return *this;
}

RenderTarget::~RenderTarget()
{
    this->_rtv->Release();
    this->_srv->Release();
    this->_texture->Release();
}

ID3D11RenderTargetView *RenderTarget::GetRTV() const
{
    return this->_rtv;
}

ID3D11ShaderResourceView *RenderTarget::GetSRV() const
{
    return this->_srv;
}

