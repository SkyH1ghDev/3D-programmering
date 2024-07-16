#pragma once

#include <d3d11.h>
#include "Configuration.hpp"
#include "D3D11Controller.hpp"
#include "InputLayout.hpp"
#include "PixelShader.hpp"
#include "RenderTargetView.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "VertexShader.hpp"

class Renderer
{
public:

    void Setup(const Configuration& configuration);

    // Forward Rendering
    void RenderForward(D3D11Controller& controller, RenderTargetView& rtv, VertexShader& vertexShader, PixelShader& pixelShader, InputLayout& inputLayout, Scene& scene, Sampler& samplerState);

    // Deferred Rendering
    void PerformGeometryPass(D3D11Controller &controller, std::vector<RenderTargetView>& rtv, VertexShader &vertexShader, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState);
    void PerformLightPass();

private:

    void ClearScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);
    void SetupInputAssembler(ID3D11DeviceContext* context, ID3D11Buffer* vertexShader, ID3D11InputLayout* inputLayout, D3D_PRIMITIVE_TOPOLOGY topology);
    void SetupVertexShader(ID3D11DeviceContext* context, ID3D11VertexShader* vertexShader, std::vector<ID3D11Buffer*> buffers);
    void SetupHullShader();
    // SetupTesselator?
    void SetupDomainShader();
    void SetupGeometryShader();
    void SetupRasterizer(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewport);
    void SetupPixelShader(ID3D11DeviceContext* context, ID3D11PixelShader* pixelShader, ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState* samplerState, std::vector<ID3D11Buffer*> buffers);
    void SetupGBuffers(ID3D11DeviceContext* context, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView**& gBuffers, size_t numGBuffers);
    void SetupOutputMerger(ID3D11DeviceContext* context, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView* rtv);

    void SetupComputeShader();

    void UnbindGBuffers(ID3D11DeviceContext* context, size_t numGBuffers);
};
