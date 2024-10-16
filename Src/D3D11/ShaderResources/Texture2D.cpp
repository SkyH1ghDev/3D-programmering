#include "Texture2D.hpp"
#include <algorithm>

Texture2D::Texture2D(ID3D11Texture2D* texture2D)
{
    this->_texture2D = texture2D;
}

Texture2D::Texture2D(const Texture2D& other)
{
    this->_texture2D = other._texture2D; this->_texture2D->AddRef();
}

Texture2D& Texture2D::operator=(const Texture2D& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->_texture2D = other._texture2D;

    return *this;
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
    std::swap(this->_texture2D, other._texture2D);
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    std::swap(this->_texture2D, other._texture2D);

    return *this;
}

Texture2D::~Texture2D()
{
    if (this->_texture2D != nullptr)
    {
        this->_texture2D->Release();
    }
}

ID3D11Texture2D* Texture2D::GetTexture2D() const
{
    return this->_texture2D;
}

