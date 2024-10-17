﻿#pragma once

#include <d3d11.h>
#include "Controller.hpp"
#include "InputLayout.hpp"
#include "RenderTargetView.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "SwapChain.hpp"

#include "VertexShader.hpp"
#include "HullShader.hpp"
#include "DomainShader.hpp"
#include "GeometryShader.hpp"
#include "Rasterizer.hpp"
#include "PixelShader.hpp"
#include "ComputeShader.hpp"
#include "StructuredBuffer.hpp"

class Renderer
{
public:
    
    // Forward Rendering
    void RenderForward(Controller& controller, RenderTargetView& rtv, VertexShader& vertexShader, Rasterizer& rasterizer, PixelShader& pixelShader, InputLayout& inputLayout, Scene& scene, Sampler& samplerState);
    
    // Deferred Rendering
    void RenderDeferred(Controller &controller, SwapChain& swapChain, RenderTargetView& rtv, std::vector<RenderTargetView>& gBuffers, VertexShader &geometryVertexShader, HullShader& geometryHullShader, DomainShader& geometryDomainShader, Rasterizer& rasterizer, PixelShader &geometryPixelShader, ComputeShader& lightComputeShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState, int& outputMode, ComputeShader& particleComputeShader, VertexShader& particleVertexShader, GeometryShader& particleGeometryShader, PixelShader& particlePixelShader, StructuredBuffer& particleBuffer);

private:
    void PerformGeometryPass(Controller &controller, std::vector<RenderTargetView>& rtv, VertexShader &vertexShader, HullShader& hullShader, DomainShader& domainShader, Rasterizer& rasterizer, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState);
    void PerformLightPass(Controller& controller, SwapChain& swapChain, std::vector<RenderTargetView>& gBuffers, ComputeShader& computeShader, Scene& scene, int& outputMode);
    void PerformParticlePass(Controller& controller, ComputeShader& computeShader, VertexShader& vertexShader, GeometryShader& geometryShader, PixelShader& pixelShader, StructuredBuffer& particleBuffer, Camera& camera, RenderTargetView& renderTargetView);

    void ClearScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);
    void ClearScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, std::vector<ID3D11RenderTargetView*> gBuffers);
    
    void SetupInputAssembler(ID3D11DeviceContext* context, ID3D11Buffer* vertexBuffer, ID3D11InputLayout* inputLayout, D3D_PRIMITIVE_TOPOLOGY topology);
    void SetupVertexShader(ID3D11DeviceContext* context, ID3D11VertexShader* vertexShader, std::vector<ConstantBuffer> buffers, DX::XMFLOAT4 meshPosition, Camera& camera);
    void SetupHullShader(ID3D11DeviceContext* context, ID3D11HullShader* hullShader, std::vector<ConstantBuffer> buffers, const DX::XMFLOAT4& meshPosition, const DX::XMFLOAT4& cameraPosition);
    // SetupTesselator?
    void SetupDomainShader(ID3D11DeviceContext* context, ID3D11DomainShader* domainShader, std::vector<ConstantBuffer>& buffers, Camera& camera);
    void SetupGeometryShader();
    void SetupRasterizer(ID3D11DeviceContext* context, ID3D11RasterizerState* rasterizerState, const D3D11_VIEWPORT& viewport);
    void SetupPixelShader(ID3D11DeviceContext* context, ID3D11PixelShader* pixelShader, ID3D11SamplerState* samplerState, std::vector<ID3D11ShaderResourceView*> textureSRVs, std::vector<ConstantBuffer> buffers, const float& specularExponent);
    void SetupGBuffers(ID3D11DeviceContext* context, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView**& gBuffers, size_t numGBuffers);
    void SetupOutputMerger(ID3D11DeviceContext* context, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView* rtv);

    void SetupComputeShader(ID3D11DeviceContext* context, ID3D11ComputeShader* computeShader, ID3D11ShaderResourceView** gBufferSRVs, size_t numGBuffers, ID3D11UnorderedAccessView* uav, std::vector<ConstantBuffer> buffers, Camera& camera, int& outputMode);

    void UnbindPipeline(ID3D11DeviceContext* context);
};
