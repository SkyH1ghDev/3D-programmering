#pragma once

#include <d3d11_4.h>

class D3D11Controller
{
public:
    D3D11Controller() = delete;
    ~D3D11Controller();
    D3D11Controller(ID3D11Device* device, ID3D11DeviceContext* immediateContext, D3D11_VIEWPORT viewport);
    D3D11Controller& operator=(const D3D11Controller& other) = delete;
    D3D11Controller& operator=(D3D11Controller&& other) noexcept;
    D3D11Controller(const D3D11Controller& other) = delete;
    D3D11Controller(const D3D11Controller&& other) = delete;
    
    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetContext();
    D3D11_VIEWPORT GetViewPort();

private:

    ID3D11Device* _device = nullptr;
    ID3D11DeviceContext* _immediateContext = nullptr;
    D3D11_VIEWPORT _viewport;
};
