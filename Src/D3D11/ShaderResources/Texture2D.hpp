#pragma once

#include <d3d11.h>

class Texture2D
{
public:
    Texture2D() = default;
    Texture2D(ID3D11Texture2D* texture2D);
    Texture2D(const Texture2D& other);
    Texture2D& operator=(const Texture2D& other);
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    ~Texture2D();
    
public:
    ID3D11Texture2D* GetTexture2D() const;
    
private:
    ID3D11Texture2D* _texture2D = nullptr;
    
};
