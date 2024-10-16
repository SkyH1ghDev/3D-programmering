#pragma once

#include <d3d11_4.h>

class ShaderResourceView
{
public:
    ShaderResourceView();
    ShaderResourceView(ShaderResourceView& other);
    ShaderResourceView& operator=(ShaderResourceView& other);
    ShaderResourceView()
public:
    ID3D11ShaderResourceView* GetSRV();
    
private:
    ID3D11ShaderResourceView* _srv = nullptr;
    
};
