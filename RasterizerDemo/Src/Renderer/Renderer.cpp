#include "Renderer.hpp"

#include <iostream>

void Renderer::RenderForward(Controller &controller, RenderTargetView &rtv, VertexShader &vertexShader, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState)
{
	ID3D11DeviceContext* context = controller.GetContext();
	ID3D11RenderTargetView* rTargetView = rtv.GetRTV();
	Camera& currCamera = scene.GetCurrentCamera();
	DepthBuffer depthBuffer = currCamera.GetDepthBuffer();

	ClearScreen(context, rTargetView, depthBuffer.GetDSV());

	for (size_t i = 0; i < scene.GetNumMeshes(); ++i)
	{
		Mesh& mesh = scene.GetMeshAt(i);
		
		VertexBuffer vertexBuffer = mesh.GetVertexBuffer();
	
		ID3D11Buffer* vBuffer = vertexBuffer.GetBuffer();
		ID3D11InputLayout* iLayout = inputLayout.GetInputLayout();
	
		SetupInputAssembler(context, vBuffer, iLayout, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11VertexShader* vShader = vertexShader.GetVertexShader();
		std::vector<ID3D11Buffer*> vertexShaderBuffers = vertexShader.GetConstantBuffers();

		SetupVertexShader(context, vShader, vertexShaderBuffers);

		ID3D11PixelShader* pShader = pixelShader.GetPixelShader();
		ID3D11ShaderResourceView* tShaderResourceView = mesh.GetTextureSRV(0);
		ID3D11SamplerState* sState = samplerState.GetSamplerState();
		std::vector<ID3D11Buffer*> pixelShaderBuffers = pixelShader.GetConstantBuffers();
	
		SetupPixelShader(context, pShader, tShaderResourceView, sState, pixelShaderBuffers);

		D3D11_VIEWPORT viewport = controller.GetViewPort();
	
		SetupRasterizer(context, viewport);

		ID3D11DepthStencilView* dsv = depthBuffer.GetDSV();
		
		SetupOutputMerger(context, dsv, rTargetView);

		for (size_t j = 0; j < mesh.GetNrOfSubMeshes(); ++j)
		{
			SubMesh& subMesh = mesh.GetSubMeshAt(j);

			if (subMesh.GetNumIndices() == 0)
			{
				continue;
			}
			context->Draw(subMesh.GetNumIndices(),subMesh.GetStartIndex());
		}
	}
}

void Renderer::RenderDeferred(Controller &controller, SwapChain& swapChain, RenderTargetView& rtv, std::vector<RenderTargetView>& gBuffers, VertexShader &vertexShader, PixelShader &pixelShader, ComputeShader& computeShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState)
{
	ID3D11DeviceContext* context = controller.GetContext();
	ID3D11RenderTargetView* rTargetView = rtv.GetRTV();
	Camera& currCamera = scene.GetCurrentCamera();
	DepthBuffer depthBuffer = currCamera.GetDepthBuffer();

	std::vector<ID3D11RenderTargetView*> gBufferRTVVec;
	for (RenderTargetView rtv : gBuffers)
	{
		gBufferRTVVec.emplace_back(rtv.GetRTV());
	}
	
	ClearScreen(context, rTargetView, depthBuffer.GetDSV(), gBufferRTVVec);
	
	PerformGeometryPass(controller, gBuffers, vertexShader, pixelShader, inputLayout, scene, samplerState);
	PerformLightPass(controller, swapChain, gBuffers, computeShader);
}


void Renderer::PerformGeometryPass(Controller &controller, std::vector<RenderTargetView>& gBuffers, VertexShader &vertexShader, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState)
{
	ID3D11DeviceContext* context = controller.GetContext();
	Camera& currCamera = scene.GetCurrentCamera();
	DepthBuffer depthBuffer = currCamera.GetDepthBuffer();

	for (size_t i = 0; i < scene.GetNumMeshes(); ++i)
	{
		Mesh& mesh = scene.GetMeshAt(i);
		
		// Setup InputAssembler
		
		VertexBuffer vertexBuffer = mesh.GetVertexBuffer();
		
		ID3D11Buffer* vBuffer = vertexBuffer.GetBuffer();
		ID3D11InputLayout* iLayout = inputLayout.GetInputLayout();
	
		SetupInputAssembler(context, vBuffer, iLayout, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Setup Vertex Shader
		
		ID3D11VertexShader* vShader = vertexShader.GetVertexShader();
		std::vector<ID3D11Buffer*> vertexShaderBuffers = vertexShader.GetConstantBuffers();
		
		SetupVertexShader(context, vShader, vertexShaderBuffers);

		// Setup Pixel Shader
		
		ID3D11PixelShader* pShader = pixelShader.GetPixelShader();
		ID3D11ShaderResourceView* tShaderResourceView = mesh.GetTextureSRV(0);
		ID3D11SamplerState* sState = samplerState.GetSamplerState();
		std::vector<ID3D11Buffer*> pixelShaderBuffers = pixelShader.GetConstantBuffers();
	
		SetupPixelShader(context, pShader, tShaderResourceView, sState, pixelShaderBuffers);

		// Setup Rasterizer
		
		D3D11_VIEWPORT viewport = controller.GetViewPort();
	
		SetupRasterizer(context, viewport);

		// Setup G-Buffers
		
		ID3D11DepthStencilView* dsv = depthBuffer.GetDSV();
		std::vector<ID3D11RenderTargetView*> gBuffersVec;

		for (RenderTargetView& gBuffer : gBuffers)
		{
			gBuffersVec.emplace_back(gBuffer.GetRTV());
		}

		ID3D11RenderTargetView** gBuffersArray = gBuffersVec.data();
		SetupGBuffers(context, dsv, gBuffersArray, gBuffersVec.size());

		// Render to G-Buffers
		
		for (size_t j = 0; j < mesh.GetNrOfSubMeshes(); ++j)
		{
			SubMesh& subMesh = mesh.GetSubMeshAt(j);

			if (subMesh.GetNumIndices() == 0)
			{
				continue;
			}
			context->Draw(subMesh.GetNumIndices(),subMesh.GetStartIndex());
		}

		UnbindGBuffersOM(context, gBuffersVec.size());
	}
}

void Renderer::PerformLightPass(Controller& controller, SwapChain& swapChain, std::vector<RenderTargetView>& gBuffers, ComputeShader& computeShader)
{
	ID3D11DeviceContext* context = controller.GetContext();
	
	size_t numSRVs = gBuffers.size();
	std::vector<ID3D11ShaderResourceView*> SRVs;
	
	for (size_t i = 0; i < numSRVs ; ++i)
	{
		SRVs.emplace_back(gBuffers.at(i).GetSRV());
	}

	ID3D11ComputeShader* cShader = computeShader.GetComputeShader();
	ID3D11ShaderResourceView** SRVarr = SRVs.data();
	ID3D11UnorderedAccessView* uav = swapChain.GetUAV();
	std::vector<ID3D11Buffer*> computeBuffers = computeShader.GetConstantBuffers();
	
	SetupComputeShader(context, cShader, SRVarr, numSRVs, uav, computeBuffers);

	context->Dispatch(160, 90, 1);

	UnbindGBuffersCS(context, numSRVs);
}


void Renderer::ClearScreen(ID3D11DeviceContext *context, ID3D11RenderTargetView *rtv, ID3D11DepthStencilView *dsv, std::vector<ID3D11RenderTargetView*> gBuffers)
{
	constexpr float clearColour[] = {0.0f, 0.0f, 0.0f, 0.0f};

	for(size_t i = 0; i < gBuffers.size(); ++i)
	{
		context->ClearRenderTargetView(gBuffers.at(i), clearColour);
	}
	context->ClearRenderTargetView(rtv, clearColour);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}


void Renderer::ClearScreen(ID3D11DeviceContext *context, ID3D11RenderTargetView *rtv, ID3D11DepthStencilView *dsv)
{
	constexpr float clearColour[] = {0.0f, 0.0f, 0.0f, 0.0f};

	context->ClearRenderTargetView(rtv, clearColour);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void Renderer::SetupInputAssembler(ID3D11DeviceContext *context, ID3D11Buffer *vertexBuffer, ID3D11InputLayout *inputLayout, D3D_PRIMITIVE_TOPOLOGY topology)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetInputLayout(inputLayout);
	context->IASetPrimitiveTopology(topology);
}

void Renderer::SetupVertexShader(ID3D11DeviceContext* context, ID3D11VertexShader* vertexShader, std::vector<ID3D11Buffer*> buffers)
{
	context->VSSetShader(vertexShader, nullptr, 0);

	ID3D11Buffer** buffersArr = buffers.data();
	context->VSSetConstantBuffers(0, buffers.size(), buffersArr);
}

void Renderer::SetupPixelShader(ID3D11DeviceContext* context, ID3D11PixelShader* pixelShader, ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState* samplerState, std::vector<ID3D11Buffer*> buffers)
{
	context->PSSetShader(pixelShader, nullptr, 0);
	context->PSSetShaderResources(0, 1, &textureSRV);
	context->PSSetSamplers(0, 1, &samplerState);

	ID3D11Buffer** buffersArr = buffers.data();
	context->PSSetConstantBuffers(0, buffers.size(), buffersArr);
}

void Renderer::SetupRasterizer(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewport)
{
	context->RSSetViewports(1, &viewport);
}

void Renderer::SetupGBuffers(ID3D11DeviceContext* context, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView**& gBuffers, size_t numGBuffers)
{
	context->OMSetRenderTargets(numGBuffers, gBuffers, dsv);
}


void Renderer::SetupOutputMerger(ID3D11DeviceContext* context, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView* rtv)
{
	context->OMSetRenderTargets(1, &rtv, dsv);
}

void Renderer::UnbindGBuffersOM(ID3D11DeviceContext* context, size_t numGBuffers)
{
	std::vector<ID3D11RenderTargetView*> nulledVec;
	for (size_t i = 0; i < numGBuffers; ++i)
	{
		nulledVec.emplace_back(nullptr);
	}

	ID3D11RenderTargetView** nulledArr = nulledVec.data();
	context->OMSetRenderTargets(numGBuffers, nulledArr, nullptr);
}

void Renderer::UnbindGBuffersCS(ID3D11DeviceContext* context, size_t numGBuffers)
{
	std::vector<ID3D11ShaderResourceView*> nulledVec;
	for (size_t i = 0; i < numGBuffers; ++i)
	{
		nulledVec.emplace_back(nullptr);
	}

	ID3D11ShaderResourceView** nulledArr = nulledVec.data();
	context->CSSetShaderResources(0, numGBuffers, nulledArr);
}

void Renderer::SetupComputeShader(ID3D11DeviceContext* context, ID3D11ComputeShader* computeShader, ID3D11ShaderResourceView** gBufferSRVs, size_t numGBuffers, ID3D11UnorderedAccessView* uav, std::vector<ID3D11Buffer*> buffers)
{
	context->CSSetShader(computeShader, nullptr, 0);
	context->CSSetShaderResources(0, numGBuffers, gBufferSRVs);
	context->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

	ID3D11Buffer** buffersArr = buffers.data();
	context->CSSetConstantBuffers(0, buffers.size(), buffersArr);
}
