#include "RenderBuffer.hpp"

#include "D3D11Helper.hpp"

RenderBuffer::RenderBuffer(ID3D11RenderTargetView *rtv, ID3D11Texture2D *texture, ID3D11ShaderResourceView* srv)
{
    this->_rtv = rtv;
    this->_texture = texture;
    this->_srv = srv;
}

RenderBuffer::RenderBuffer(ID3D11RenderTargetView* rtv)
{
    this->_rtv = rtv;
    this->_texture = nullptr;
    this->_srv = nullptr;
}

ID3D11RenderTargetView* RenderBuffer::GetRTV() const
{
    return this->_rtv.GetRTV();
}

ID3D11ShaderResourceView* RenderBuffer::GetSRV() const
{
    return this->_srv.GetSRV();
}

