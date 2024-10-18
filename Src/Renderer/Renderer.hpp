#pragma once

#include <d3d11.h>
#include "Controller.hpp"
#include "InputLayout.hpp"
#include "RenderBuffer.hpp"
#include "Sampler.hpp"
#include "Scene.hpp"
#include "SwapChain.hpp"
#include "SpotLight.hpp"
#include "ShadowMapCollection.hpp"
#include "CubeMapCollection.hpp"

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
    void RenderForward(Controller& controller, RenderBuffer& rtv, VertexShader& vertexShader, Rasterizer& rasterizer, PixelShader& pixelShader, InputLayout& inputLayout, Scene& scene, Sampler& samplerState);
    
    // Deferred Rendering
    void RenderDeferred(Controller &controller, SwapChain& swapChain, RenderBuffer& rtv, std::vector<RenderBuffer>& gBuffers, VertexShader &geometryVertexShader, HullShader& geometryHullShader, DomainShader& geometryDomainShader, Rasterizer& rasterizer, PixelShader &geometryPixelShader, ComputeShader& lightComputeShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState, int& outputMode, ComputeShader& particleComputeShader, VertexShader& particleVertexShader, GeometryShader& particleGeometryShader, PixelShader& particlePixelShader, StructuredBuffer& particleBuffer, std::vector<SpotLight>& spotlights, ShadowMapCollection& shadowMaps, StructuredBuffer& spotlightsBuffer, Sampler& depthSampler, D3D11_VIEWPORT cubeMapViewport, std::vector<Camera>& cubeMapCameras, CubeMapCollection& cubeMaps, std::vector<RenderBuffer>& cubeMapGBuffers, PixelShader& reflectionPixelShader, ComputeShader& reflectionComputeShader);

private:
    void PerformShadowPass(Controller& controller, InputLayout& inputLayout, VertexShader& vertexShader, std::vector<SpotLight>& spotlights, ShadowMapCollection& shadowMaps, Scene& scene);
    void PerformShadowPass(Controller& controller, InputLayout& inputLayout, VertexShader& vertexShader, std::vector<SpotLight>& spotlights, ShadowMapCollection& shadowMaps, Scene& scene, D3D11_VIEWPORT viewport);
    
    void PerformGeometryPass(Controller &controller, std::vector<RenderBuffer>& rtv, VertexShader &vertexShader, HullShader& hullShader, DomainShader& domainShader, Rasterizer& rasterizer, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState, PixelShader& reflectionPixelShader, CubeMapCollection& cubeMap);
    void PerformGeometryPass(Controller &controller, std::vector<RenderBuffer>& rtv, VertexShader &vertexShader, HullShader& hullShader, DomainShader& domainShader, Rasterizer& rasterizer, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState, Camera& currCamera, D3D11_VIEWPORT viewport);
    
    void PerformLightPass(Controller& controller, SwapChain& swapChain, std::vector<RenderBuffer>& gBuffers, ComputeShader& computeShader, Scene& scene, int& outputMode, StructuredBuffer& spotlights, ShadowMapCollection& shadowMaps, Sampler& depthSampler);
    void PerformLightPass(Controller& controller, SwapChain& swapChain, std::vector<RenderBuffer>& gBuffers, ComputeShader& computeShader, Scene& scene, int& outputMode, StructuredBuffer& spotlights, ShadowMapCollection& shadowMaps, Sampler& depthSampler, Camera& currCamera, ID3D11UnorderedAccessView* uav, const int& cubeMapIndex);
    
    void PerformParticlePass(Controller& controller, ComputeShader& computeShader, VertexShader& vertexShader, GeometryShader& geometryShader, PixelShader& pixelShader, StructuredBuffer& particleBuffer, Camera& camera, RenderBuffer& renderTargetView);

    void PerformReflectionPass();
    
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

    void SetupComputeShader(ID3D11DeviceContext* context, ID3D11ComputeShader* computeShader, ID3D11ShaderResourceView** SRVs, size_t numSRVs, ID3D11UnorderedAccessView* uav, std::vector<ConstantBuffer> buffers, Camera& camera, int& outputMode);

    void UnbindPipeline(ID3D11DeviceContext* context);
};
