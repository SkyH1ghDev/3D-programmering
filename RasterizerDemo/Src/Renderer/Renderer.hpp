#pragma once

#include <d3d11.h>
#include "Configuration.hpp"
#include "D3D11Controller.hpp"
#include "InputLayout.hpp"
#include "PixelShader.hpp"
#include "RenderTarget.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "VertexShader.hpp"

class Renderer
{
public:

    void Setup(const Configuration& configuration);
    void Render(D3D11Controller& controller, RenderTarget& rtv, VertexShader& vertexShader, PixelShader& pixelShader, InputLayout& inputLayout, Scene& scene, Sampler& samplerState);

private:

    void ClearScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);
    void SetupInputAssembler(ID3D11DeviceContext* context, ID3D11Buffer* vertexShader, ID3D11InputLayout* inputLayout, D3D_PRIMITIVE_TOPOLOGY topology);
    void SetupVertexShader(ID3D11DeviceContext* context, ID3D11VertexShader* vertexShader);
    void SetupHullShader();
    // SetupTesselator?
    void SetupDomainShader();
    void SetupGeometryShader();
    void SetupRasterizer(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewport);
    void SetupPixelShader(ID3D11DeviceContext* context, ID3D11PixelShader* pixelShader, ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState* samplerState);
    void SetupOutputMerger(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);

    void SetupComputeShader();
};
