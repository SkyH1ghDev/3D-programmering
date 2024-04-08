﻿#pragma once

#include <d3d11.h>
#include "Configuration.hpp"
class Renderer
{
public:

   void Setup(const Configuration& configuration);
    
    void Render(ID3D11DeviceContext* immediateContext, ID3D11RenderTargetView* rtv,
                ID3D11DepthStencilView* dsView, D3D11_VIEWPORT& viewport, ID3D11VertexShader* vShader,
                ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout, ID3D11Buffer* vertexBuffer,
                ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState* samplerState);
};
