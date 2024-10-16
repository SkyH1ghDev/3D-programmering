#pragma once

#include <d3d11_3.h>

class ShaderResourceView
{
public:
    ShaderResourceView() = default;
    ShaderResourceView(ID3D11ShaderResourceView* srv);
    ShaderResourceView(const ShaderResourceView& other);
    ShaderResourceView& operator=(const ShaderResourceView& other);
    ShaderResourceView(ShaderResourceView&& other) noexcept;
    ShaderResourceView& operator=(ShaderResourceView&& other) noexcept;

    ~ShaderResourceView();
    
public:
    ID3D11ShaderResourceView* GetSRV() const;
    
private:
    ID3D11ShaderResourceView* _srv = nullptr;
    
};
