#include "RenderTargetView.hpp"

#include "D3D11Helper.hpp"

RenderTargetView::RenderTargetView(ID3D11RenderTargetView *rtv, ID3D11Texture2D *texture, ID3D11ShaderResourceView* srv)
{
    this->_rtv = rtv;
    this->_texture = texture;
    this->_srv = srv;
}

RenderTargetView::RenderTargetView(ID3D11RenderTargetView* rtv)
{
    this->_rtv = rtv;
}

RenderTargetView::RenderTargetView(const RenderTargetView& other)
{
    this->_rtv = other._rtv; this->_rtv->AddRef();
    this->_texture = other._texture; this->_texture->AddRef();
    this->_srv = other._srv; this->_srv->AddRef();
}

RenderTargetView::RenderTargetView(RenderTargetView&& other) noexcept
{
    this->_rtv = other._rtv; this->_rtv->AddRef();
    this->_texture = other._texture; this->_texture->AddRef();
    this->_srv = other._srv; this->_srv->AddRef();
}

RenderTargetView& RenderTargetView::operator=(const RenderTargetView& other)
{
    if (&other == this)
    {
        return *this;
    }

    this->_rtv = other._rtv; this->_rtv->AddRef();
    this->_texture = other._texture; this->_texture->AddRef();
    this->_srv = other._srv; this->_srv->AddRef();
    
    return *this;
}


RenderTargetView& RenderTargetView::operator=(RenderTargetView&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }

    this->_rtv = other._rtv; this->_rtv->AddRef();
    this->_texture = other._texture; this->_texture->AddRef();
    this->_srv = other._srv; this->_srv->AddRef();
    
    return *this;
}

RenderTargetView::~RenderTargetView()
{
    if (this->_texture != nullptr) { this->_texture->Release(); }
    this->_rtv->Release();
    if (this->_srv!= nullptr) { this->_srv->Release(); }
}

ID3D11RenderTargetView* RenderTargetView::GetRTV() const
{
    return this->_rtv;
}

ID3D11ShaderResourceView* RenderTargetView::GetSRV() const
{
    return this->_srv;
}

