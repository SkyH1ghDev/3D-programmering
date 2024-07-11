﻿#include "Renderer.hpp"

#include <iostream>

void Renderer::RenderForward(D3D11Controller &controller, RenderTargetView &rtv, VertexShader &vertexShader, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState)
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

		SetupVertexShader(context, vShader);

		ID3D11PixelShader* pShader = pixelShader.GetPixelShader();
		ID3D11ShaderResourceView* tShaderResourceView = mesh.GetTextureSRV(0);
		ID3D11SamplerState* sState = samplerState.GetSamplerState();
	
		SetupPixelShader(context, pShader, tShaderResourceView, sState);

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

void Renderer::PerformGeometryPass(D3D11Controller &controller, std::vector<RenderTargetView>& gBuffers, VertexShader &vertexShader, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState)
{
	ID3D11DeviceContext* context = controller.GetContext();
	Camera& currCamera = scene.GetCurrentCamera();
	DepthBuffer depthBuffer = currCamera.GetDepthBuffer();

	for (size_t i = 0; i < scene.GetNumMeshes(); ++i)
	{
		Mesh& mesh = scene.GetMeshAt(i);
		
		VertexBuffer vertexBuffer = mesh.GetVertexBuffer();
	
		ID3D11Buffer* vBuffer = vertexBuffer.GetBuffer();
		ID3D11InputLayout* iLayout = inputLayout.GetInputLayout();
	
		SetupInputAssembler(context, vBuffer, iLayout, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11VertexShader* vShader = vertexShader.GetVertexShader();

		SetupVertexShader(context, vShader);

		ID3D11PixelShader* pShader = pixelShader.GetPixelShader();
		ID3D11ShaderResourceView* tShaderResourceView = mesh.GetTextureSRV(0);
		ID3D11SamplerState* sState = samplerState.GetSamplerState();
	
		SetupPixelShader(context, pShader, tShaderResourceView, sState);

		D3D11_VIEWPORT viewport = controller.GetViewPort();
	
		SetupRasterizer(context, viewport);
		
		ID3D11DepthStencilView* dsv = depthBuffer.GetDSV();
		std::vector<ID3D11RenderTargetView*> gBuffersVec;

		for (RenderTargetView& gBuffer : gBuffers)
		{
			gBuffersVec.emplace_back(gBuffer.GetRTV());
		}

		ID3D11RenderTargetView** gBuffersArray = gBuffersVec.data();
		SetupGBuffers(context, dsv, gBuffersArray, gBuffersVec.size());

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

void Renderer::SetupVertexShader(ID3D11DeviceContext* context, ID3D11VertexShader* vertexShader)
{
	context->VSSetShader(vertexShader, nullptr, 0);
}

void Renderer::SetupPixelShader(ID3D11DeviceContext* context, ID3D11PixelShader* pixelShader, ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState* samplerState)
{
	context->PSSetShader(pixelShader, nullptr, 0);
	context->PSSetShaderResources(0, 1, &textureSRV);
	context->PSSetSamplers(0, 1, &samplerState);
}

void Renderer::SetupRasterizer(ID3D11DeviceContext* context, const D3D11_VIEWPORT& viewport)
{
	context->RSSetViewports(1, &viewport);
}

void Renderer::SetupGBuffers(ID3D11DeviceContext* context, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView**& gBuffers, size_t numGBuffers)
{
	context->OMSetRenderTargets(numGBuffers, &gBuffers[0], dsv);
}


void Renderer::SetupOutputMerger(ID3D11DeviceContext* context, ID3D11DepthStencilView* dsv, ID3D11RenderTargetView* rtv)
{
	context->OMSetRenderTargets(1, &rtv, dsv);
}

