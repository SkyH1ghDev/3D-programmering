#include "SwapChain.hpp"

SwapChain::SwapChain(IDXGISwapChain* swapChain, ID3D11Texture2D* backBuffer, ID3D11UnorderedAccessView* uav)
{
    this->_swapChain = swapChain;
    this->_backBuffer = backBuffer;
    this->_uav = uav;
}

SwapChain::~SwapChain()
{
    this->_swapChain->Release();
}

SwapChain& SwapChain::operator=(SwapChain&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }
    
    this->_swapChain = other._swapChain; this->_swapChain->AddRef();
    this->_backBuffer = other._backBuffer;
    this->_uav = other._uav;

    return *this;
}


IDXGISwapChain* SwapChain::GetSwapChain()
{
    return this->_swapChain;
}

ID3D11Texture2D* SwapChain::GetBackBuffer()
{
    return this->_backBuffer.GetTexture2D();
}


ID3D11UnorderedAccessView* SwapChain::GetUAV()
{
    return this->_uav.GetUAV();
}


