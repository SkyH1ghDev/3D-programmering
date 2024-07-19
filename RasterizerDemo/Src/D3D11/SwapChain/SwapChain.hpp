#pragma once

#include <d3d11_4.h>

class SwapChain
{
public:
    SwapChain() = delete;
    ~SwapChain();
    SwapChain(IDXGISwapChain* swapChain, ID3D11Texture2D* backBuffer, ID3D11UnorderedAccessView* uav);
    SwapChain& operator=(const SwapChain& other) = delete;
    SwapChain& operator=(SwapChain&& other) noexcept;
    SwapChain(const SwapChain& other) = delete;
    SwapChain(const SwapChain&& other) = delete;

    IDXGISwapChain* GetSwapChain();
    ID3D11Texture2D* GetBackBuffer();
    ID3D11UnorderedAccessView* GetUAV();
    
private:
    IDXGISwapChain* _swapChain = nullptr;
    ID3D11Texture2D* _backBuffer = nullptr;
    ID3D11UnorderedAccessView* _uav = nullptr;
};
