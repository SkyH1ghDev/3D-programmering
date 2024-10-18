#pragma once

#include <vector>

#include "Texture2D.hpp"
#include "ShaderResourceView.hpp"
#include "RenderTargetView.hpp"
#include "UnorderedAccessView.hpp"

class CubeMapCollection
{
public:
    CubeMapCollection(HRESULT& hr, ID3D11Device* device);
    
public:
    Texture2D GetTexture2DArray();
    ShaderResourceView GetShaderResourceView();
    ID3D11UnorderedAccessView* GetUnorderedAccessViewAt(UINT index);
    ID3D11RenderTargetView* GetRenderTargetViewAt(UINT index);

private:
    Texture2D _texture;
    ShaderResourceView _srv;
    std::vector<RenderTargetView> _rtvs;
    std::vector<UnorderedAccessView> _uavs;
    
};
