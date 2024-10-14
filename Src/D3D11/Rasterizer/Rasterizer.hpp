#pragma once
#include "D3D11Helper.hpp"

class Rasterizer
{
public:
    Rasterizer(ID3D11RasterizerState* rasterizerState);
    ~Rasterizer();

public:
    ID3D11RasterizerState* GetRasterizerState();
    
private:
    ID3D11RasterizerState* _rasterizerState;
};
