#include "RenderTargetView.hpp"

#include <algorithm>

RenderTargetView::RenderTargetView(ID3D11RenderTargetView* rtv)
{
    this->_rtv = rtv;
}

RenderTargetView::RenderTargetView(const RenderTargetView& other)
{
    this->_rtv = other._rtv; this->_rtv->AddRef();
}

RenderTargetView& RenderTargetView::operator=(const RenderTargetView& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->_rtv = other._rtv; this->_rtv->AddRef();

    return *this;
}

RenderTargetView::RenderTargetView(RenderTargetView&& other) noexcept
{
    std::swap(this->_rtv, other._rtv);
}

RenderTargetView& RenderTargetView::operator=(RenderTargetView&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    std::swap(this->_rtv, other._rtv);

    return *this;
}

RenderTargetView::~RenderTargetView()
{
    if (this->_rtv != nullptr)
    {
        this->_rtv->Release();
    }
}

ID3D11RenderTargetView* RenderTargetView::GetRTV() const
{
    return this->_rtv;
}
