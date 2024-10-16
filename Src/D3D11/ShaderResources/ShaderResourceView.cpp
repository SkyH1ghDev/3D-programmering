#include "ShaderResourceView.hpp"
#include <algorithm>

ShaderResourceView::ShaderResourceView(ID3D11ShaderResourceView* srv)
{
    this->_srv = srv;
}

ShaderResourceView::ShaderResourceView(const ShaderResourceView& other)
{
    this->_srv = other._srv; this->_srv->AddRef();
}

ShaderResourceView& ShaderResourceView::operator=(const ShaderResourceView& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->_srv = other._srv; this->_srv->AddRef();

    return *this;
}

ShaderResourceView::ShaderResourceView(ShaderResourceView&& other) noexcept
{
    std::swap(this->_srv, other._srv);
}

ShaderResourceView& ShaderResourceView::operator=(ShaderResourceView&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }
    
    std::swap(this->_srv, other._srv);
    
    return *this;
}

ShaderResourceView::~ShaderResourceView()
{
    if (this->_srv != nullptr)
    {
        this->_srv->Release();
    }
}

ID3D11ShaderResourceView* ShaderResourceView::GetSRV() const
{
    return this->_srv;
}






