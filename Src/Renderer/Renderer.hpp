#pragma once

#include <d3d11.h>
#include <array>
#include "Configuration.hpp"
#include "Controller.hpp"
#include "InputLayout.hpp"
#include "PixelShader.hpp"
#include "RenderTargetView.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "SwapChain.hpp"
#include "VertexShader.hpp"
#include "ComputeShader.hpp"

class Renderer
{
public:
    
    // Forward Rendering
    void RenderForward(Controller& controller, RenderTargetView& rtv, VertexShader& vertexShader, PixelShader& pixelShader, InputLayout& inputLayout, Scene& scene, Sampler& samplerState);
    
    // Deferred Rendering
    void RenderDeferred(Controller &controller, SwapChain& swapChain, RenderTargetView& rtv, std::vector<RenderTargetView>& gBuffers, VertexShader &vertexShader, PixelShader &pixelShader, ComputeShader& computeShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState, int& outputMode);

private:
    void PerformGeometryPass(Controller &controller, std::vector<RenderTargetView>& rtv, VertexShader &vertexShader, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState);
    void PerformLightPass(Controller& controller, SwapChain& swapChain, std::vector<RenderTargetView>& gBuffers, ComputeShader& computeShader, Scene& scene, int& outputMode);

    void ClearScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);
    void ClearScreen(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, std::vector<ID3D11RenderTargetView*> gBuffers);
    
    void SetupInputAssembler(ID3D11DeviceContext* context, ID3D11Buffer* vertexShader, ID3D11InputLayout* inputLayout, D3D_PRIMITIVE_TOPOLOGY topology);
    void SetupVertexShader(ID3D11DeviceContext* context, ID3D11VertexShader* vertexShader, std::vector<ConstantBuffer> buffers, std::array<float, 4> meshPosition, Camera& camera);
    void SetupHullShader();
    // SetupTesselator?
    void SetupDomainShader();
    void SetupGeometryShader();
    void SetupRasterizer(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewport);
    void SetupPixelShader(ID3D11DeviceContext* context, ID3D11PixelShader* pixelShader, ID3D11SamplerState* samplerState, std::vector<ID3D11ShaderResourceView*> textureSRVs, std::vector<ConstantBuffer> buffers, const float& specularExponent);
    void SetupGBuffers(ID3D11DeviceContext* context, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView**& gBuffers, size_t numGBuffers);
    void SetupOutputMerger(ID3D11DeviceContext* context, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView* rtv);

    void SetupComputeShader(ID3D11DeviceContext* context, ID3D11ComputeShader* computeShader, ID3D11ShaderResourceView** gBufferSRVs, size_t numGBuffers, ID3D11UnorderedAccessView* uav, std::vector<ConstantBuffer> buffers, Camera& camera, int& outputMode);

    void UnbindGBuffersOM(ID3D11DeviceContext* context, size_t numGBuffers);
    void UnbindGBuffersCS(ID3D11DeviceContext* context, size_t numGBuffers);
};
