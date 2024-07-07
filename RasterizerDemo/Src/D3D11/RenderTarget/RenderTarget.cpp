#include "RenderTarget.hpp"

#include "D3D11Helper.hpp"

// TODO: Refactor RenderTarget(device, swapChain) to SetupHelper instead.

RenderTarget::RenderTarget(ID3D11RenderTargetView *rtv, ID3D11Texture2D *dsTexture, ID3D11DepthStencilView *dsv)
{
    this->_rtv = rtv;
    this->_texture = dsTexture;
    this->_dsv = dsv;
}


RenderTarget& RenderTarget::operator=(RenderTarget&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    this->_rtv = other._rtv; this->_rtv->AddRef();
    this->_texture = other._texture; this->_texture->AddRef();
    this->_dsv = other._dsv; this->_dsv->AddRef();
    
    return *this;
}

RenderTarget::~RenderTarget()
{
    this->_rtv->Release();
    this->_dsv->Release();
    this->_texture->Release();
}

ID3D11RenderTargetView *RenderTarget::GetRTV() const
{
    return this->_rtv;
}

ID3D11DepthStencilView *RenderTarget::GetDSV() const
{
    return this->_dsv;
}

