#pragma once

#include <windows.h>
#include <vector>

#include "Texture2D.hpp"
#include "DepthStencilView.hpp"
#include "ShaderResourceView.hpp"

class ShadowMapCollection
{
public:
    ShadowMapCollection(HRESULT& hr, ID3D11Device* device, const UINT& arraySize);

public:
    Texture2D GetTexture2DArray();
    ShaderResourceView GetShaderResourceView();
    DepthStencilView GetDepthStencilViewAt(UINT index);
    
private:
    Texture2D _texture;
    ShaderResourceView _srv;
    std::vector<DepthStencilView> _dsvs;
};
