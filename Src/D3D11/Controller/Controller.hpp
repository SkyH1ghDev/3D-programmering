#pragma once

#include <d3d11_4.h>

class Controller
{
public:
    Controller() = delete;
    ~Controller();
    Controller(ID3D11Device* device, ID3D11DeviceContext* immediateContext, D3D11_VIEWPORT viewport);
    Controller& operator=(const Controller& other) = delete;
    Controller& operator=(Controller&& other) noexcept;
    Controller(const Controller& other) = delete;
    Controller(const Controller&& other) = delete;
    
    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetContext();
    D3D11_VIEWPORT GetViewPort();

private:

    ID3D11Device* _device = nullptr;
    ID3D11DeviceContext* _immediateContext = nullptr;
    D3D11_VIEWPORT _viewport;
};
