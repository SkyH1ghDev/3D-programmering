#pragma once

#include <d3d11.h>

class DepthStencilView
{
public:
    DepthStencilView() = default;
    DepthStencilView(ID3D11DepthStencilView* srv);
    DepthStencilView(const DepthStencilView& other);
    DepthStencilView& operator=(const DepthStencilView& other);
    DepthStencilView(DepthStencilView&& other) noexcept;
    DepthStencilView& operator=(DepthStencilView&& other) noexcept;

    ~DepthStencilView();
    
public:
    ID3D11DepthStencilView* GetDSV() const;
    
private:
    ID3D11DepthStencilView* _dsv = nullptr;
};
