#include "DepthStencilView.hpp"

#include <algorithm>

DepthStencilView::DepthStencilView(ID3D11DepthStencilView* dsv)
{
    this->_dsv = dsv;
}

DepthStencilView::DepthStencilView(const DepthStencilView& other)
{
    this->_dsv = other._dsv; this->_dsv->AddRef();
}

DepthStencilView& DepthStencilView::operator=(const DepthStencilView& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->_dsv = other._dsv; this->_dsv->AddRef();

    return *this;
}

DepthStencilView::DepthStencilView(DepthStencilView&& other) noexcept
{
    std::swap(this->_dsv, other._dsv);
}

DepthStencilView& DepthStencilView::operator=(DepthStencilView&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    std::swap(this->_dsv, other._dsv);

    return *this;
}

DepthStencilView::~DepthStencilView()
{
    if (this->_dsv != nullptr)
    {
        this->_dsv->Release();
    }
}

ID3D11DepthStencilView* DepthStencilView::GetDSV() const
{
    return this->_dsv;
}