#include "Rasterizer.hpp"

Rasterizer::Rasterizer(ID3D11RasterizerState* rasterizerState)
{
    this->_rasterizerState = rasterizerState;
}

Rasterizer::~Rasterizer()
{
    this->_rasterizerState->Release();
}

ID3D11RasterizerState* Rasterizer::GetRasterizerState()
{
    return this->_rasterizerState;
}


