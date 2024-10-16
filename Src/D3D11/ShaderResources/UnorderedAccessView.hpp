#pragma once

#include <d3d11_3.h>

class UnorderedAccessView
{
public:
    UnorderedAccessView() = default;
    UnorderedAccessView(ID3D11UnorderedAccessView* uav);
    UnorderedAccessView(const UnorderedAccessView& other);
    UnorderedAccessView& operator=(const UnorderedAccessView& other);
    UnorderedAccessView(UnorderedAccessView&& other) noexcept;
    UnorderedAccessView& operator=(UnorderedAccessView&& other) noexcept;

    ~UnorderedAccessView();
    
public:
    ID3D11UnorderedAccessView* GetUAV() const;
    
private:
    ID3D11UnorderedAccessView* _uav = nullptr;
    
};

