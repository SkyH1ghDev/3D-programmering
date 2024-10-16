#pragma once

#include "Texture2D.hpp"
#include "UnorderedAccessView.hpp"

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
    Texture2D _backBuffer;
    UnorderedAccessView _uav;
};
