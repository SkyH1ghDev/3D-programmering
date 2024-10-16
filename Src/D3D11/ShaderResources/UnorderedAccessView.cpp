#include "UnorderedAccessView.hpp"
#include <algorithm>

UnorderedAccessView::UnorderedAccessView(ID3D11UnorderedAccessView* uav)
{
    this->_uav = uav;
}

UnorderedAccessView::UnorderedAccessView(const UnorderedAccessView& other)
{
    this->_uav = other._uav; this->_uav->AddRef();
}

UnorderedAccessView& UnorderedAccessView::operator=(const UnorderedAccessView& other)
{
    if (this == &other)
    {
        return *this;
    }
    
    this->_uav = other._uav; this->_uav->AddRef();

    return *this;
}

UnorderedAccessView::UnorderedAccessView(UnorderedAccessView&& other) noexcept
{
    std::swap(this->_uav, other._uav);
}

UnorderedAccessView& UnorderedAccessView::operator=(UnorderedAccessView&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    std::swap(this->_uav, other._uav);

    return *this;
}

UnorderedAccessView::~UnorderedAccessView()
{
    if (this->_uav != nullptr)
    {
        this->_uav->Release();
    }
}

ID3D11UnorderedAccessView* UnorderedAccessView::GetUAV() const
{
    return this->_uav;
}