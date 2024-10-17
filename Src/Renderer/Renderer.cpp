#include "Renderer.hpp"

#include <iostream>

#include "VSGeometryData.hpp"
#include "HSGeometryData.hpp"
#include "DSGeometryData.hpp"
#include "PSGeometryData.hpp"
#include "CSLightData.hpp"
#include "GSParticleData.hpp"

#include "LightConfig.hpp"
#include "MatrixCreator.hpp"

void Renderer::RenderForward(Controller &controller, RenderTargetView &rtv, VertexShader &vertexShader, Rasterizer& rasterizer, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState)
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
		std::vector<ConstantBuffer> vertexShaderBuffers = vertexShader.GetConstantBuffers();

		SetupVertexShader(context, vShader, vertexShaderBuffers, mesh.GetCurrentPosition(), currCamera);



		D3D11_VIEWPORT viewport = controller.GetViewPort();
		ID3D11RasterizerState* rasterizerState = rasterizer.GetRasterizerState();
		SetupRasterizer(context, rasterizerState, viewport);

		ID3D11DepthStencilView* dsv = depthBuffer.GetDSV();
		
		SetupOutputMerger(context, dsv, rTargetView);

		for (size_t j = 0; j < mesh.GetNrOfSubMeshes(); ++j)
		{
			ID3D11PixelShader* pShader = pixelShader.GetPixelShader();
			std::vector<ID3D11ShaderResourceView*> tShaderResourceViews = {mesh.GetTextureSRV(j)};
			ID3D11SamplerState* sState = samplerState.GetSamplerState();
			std::vector<ConstantBuffer> pixelShaderBuffers = pixelShader.GetConstantBuffers();
		
			SubMesh& subMesh = mesh.GetSubMeshAt(j);

			SetupPixelShader(context, pShader, sState, tShaderResourceViews, pixelShaderBuffers, subMesh.GetSpecularExponent());
			
			if (subMesh.GetNumIndices() == 0)
			{
				continue;
			}
			context->Draw(subMesh.GetNumIndices(),subMesh.GetStartIndex());
		}
	}
}

void Renderer::RenderDeferred(Controller &controller, SwapChain& swapChain, RenderTargetView& rtv, std::vector<RenderTargetView>& gBuffers, VertexShader &geometryVertexShader, HullShader& geometryHullShader, DomainShader& geometryDomainShader, Rasterizer& rasterizer, PixelShader &geometryPixelShader, ComputeShader& lightComputeShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState, int& outputMode, ComputeShader& particleComputeShader, VertexShader& particleVertexShader, GeometryShader& particleGeometryShader, PixelShader& particlePixelShader, StructuredBuffer& particleBuffer)
{
	ID3D11DeviceContext* context = controller.GetContext();
	ID3D11RenderTargetView* rTargetView = rtv.GetRTV();
	Camera& currCamera = scene.GetCurrentCamera();
	DepthBuffer depthBuffer = currCamera.GetDepthBuffer();

	std::vector<ID3D11RenderTargetView*> gBufferRTVVec;
	for (RenderTargetView renderTargetView : gBuffers)
	{
		gBufferRTVVec.emplace_back(renderTargetView.GetRTV());
	}
	
	ClearScreen(context, rTargetView, depthBuffer.GetDSV(), gBufferRTVVec);
	
	PerformGeometryPass(controller, gBuffers, geometryVertexShader, geometryHullShader, geometryDomainShader, rasterizer, geometryPixelShader, inputLayout, scene, samplerState);
	UnbindPipeline(context);
	PerformLightPass(controller, swapChain, gBuffers, lightComputeShader, scene, outputMode);
	UnbindPipeline(context);
	PerformParticlePass(controller, particleComputeShader, particleVertexShader, particleGeometryShader, particlePixelShader, particleBuffer, scene.GetMainCamera(), rtv);
	UnbindPipeline(context);
}

void Renderer::PerformGeometryPass(Controller &controller, std::vector<RenderTargetView>& gBuffers, VertexShader &vertexShader, HullShader& hullShader, DomainShader& domainShader, Rasterizer& rasterizer, PixelShader &pixelShader, InputLayout &inputLayout, Scene& scene, Sampler& samplerState)
{
	ID3D11DeviceContext* context = controller.GetContext();
	Camera& currCamera = scene.GetCurrentCamera();
	DepthBuffer depthBuffer = currCamera.GetDepthBuffer();
	const QuadTree<Mesh>& quadTree = scene.GetQuadTree();

	std::vector<std::shared_ptr<Mesh>> meshesToRender = quadTree.CheckTree(scene.GetMainCamera().GetBoundingFrustum());
	
	for (size_t i = 0; i < scene.GetNumOscillatingMeshes(); ++i)
	{
		meshesToRender.push_back(scene.GetOscillatingMeshPtrAt(i));
	}
	
	for (std::shared_ptr<Mesh> meshPtr : meshesToRender)
	{
		// Setup InputAssembler
		
		VertexBuffer vertexBuffer = meshPtr->GetVertexBuffer();
		
		ID3D11Buffer* vBuffer = vertexBuffer.GetBuffer();
		ID3D11InputLayout* iLayout = inputLayout.GetInputLayout();
	
		SetupInputAssembler(context, vBuffer, iLayout, D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

		// Setup Vertex Shader
		
		ID3D11VertexShader* vShader = vertexShader.GetVertexShader();
		std::vector<ConstantBuffer> vertexShaderBuffers = vertexShader.GetConstantBuffers();
		
		SetupVertexShader(context, vShader, vertexShaderBuffers, meshPtr->GetCurrentPosition(), currCamera);

		ID3D11HullShader* hShader = hullShader.GetHullShader();
		std::vector<ConstantBuffer> hullShaderBuffers = hullShader.GetConstantBuffers();
		
		SetupHullShader(context, hShader, hullShaderBuffers, meshPtr->GetCurrentPosition(), scene.GetMainCamera().GetPosition());

		ID3D11DomainShader* dShader = domainShader.GetDomainShader();
		std::vector<ConstantBuffer> domainShaderBuffers = domainShader.GetConstantBuffers();
		
		SetupDomainShader(context, dShader, domainShaderBuffers, scene.GetCurrentCamera());
		
		// Setup Rasterizer
		
		D3D11_VIEWPORT viewport = controller.GetViewPort();
		ID3D11RasterizerState* rasterizerState = rasterizer.GetRasterizerState();
		
		SetupRasterizer(context, rasterizerState, viewport);

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
		
		for (size_t j = 0; j < meshPtr->GetNrOfSubMeshes(); ++j)
		{
			ID3D11PixelShader* pShader = pixelShader.GetPixelShader();
			std::vector<ID3D11ShaderResourceView*> tShaderResourceViews = {meshPtr->GetTextureSRV(j), meshPtr->GetAmbientSRV(j), meshPtr->GetDiffuseSRV(j), meshPtr->GetSpecularSRV(j)};
			ID3D11SamplerState* sState = samplerState.GetSamplerState();
			std::vector<ConstantBuffer> pixelShaderBuffers = pixelShader.GetConstantBuffers();
         	
			SubMesh& subMesh = meshPtr->GetSubMeshAt(j);

			SetupPixelShader(context, pShader, sState, tShaderResourceViews, pixelShaderBuffers, subMesh.GetSpecularExponent());
			
			if (subMesh.GetNumIndices() == 0)
			{
				continue;
			}
			context->Draw(subMesh.GetNumIndices(),subMesh.GetStartIndex());
		}
	}
}

void Renderer::PerformLightPass(Controller& controller, SwapChain& swapChain, std::vector<RenderTargetView>& gBuffers, ComputeShader& computeShader, Scene& scene, int& outputMode)
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
	std::vector<ConstantBuffer> computeConstantBuffers = computeShader.GetConstantBuffers();
	
	SetupComputeShader(context, cShader, SRVarr, numSRVs, uav, computeConstantBuffers, scene.GetCurrentCamera(), outputMode);

	context->Dispatch(160, 90, 1);
}

void Renderer::PerformParticlePass(Controller& controller, ComputeShader& computeShader, VertexShader& vertexShader, GeometryShader& geometryShader, PixelShader& pixelShader, StructuredBuffer& particleBuffer, Camera& camera, RenderTargetView& renderTargetView)
{
	ID3D11DeviceContext* context = controller.GetContext();
	
	ID3D11ComputeShader* cShader = computeShader.GetComputeShader();
	ID3D11UnorderedAccessView* uav = particleBuffer.GetUAV();
	context->CSSetShader(cShader, nullptr, 0);
	context->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);
	
	context->Dispatch(particleBuffer.GetNrOfElements(), 1, 1);
	
	UnbindPipeline(context);

	// IA
	
	context->IASetInputLayout(nullptr);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// VS
	
	ID3D11VertexShader* vShader = vertexShader.GetVertexShader();
	ID3D11ShaderResourceView* srv = particleBuffer.GetSRV();
	context->VSSetShader(vShader, nullptr, 0);
	context->VSSetShaderResources(0, 1, &srv);

	// GS
	
	ID3D11GeometryShader* gShader = geometryShader.GetGeometryShader();
	context->GSSetShader(gShader, nullptr, 0);

	MatrixCreator matrixCreator;
	
	DX::XMMATRIX viewProjMatrix = matrixCreator.CreateViewProjectionMatrix(camera);
	DX::XMFLOAT4X4 viewProjectionMatrix;
	DX::XMStoreFloat4x4(&viewProjectionMatrix, viewProjMatrix);
	
	GSParticleData gsParticleData;
	gsParticleData.ViewProjectionMatrix = viewProjectionMatrix;
	gsParticleData.CameraPosition = camera.GetPosition();

	std::vector<ConstantBuffer> gsBuffers = geometryShader.GetConstantBuffers();
	gsBuffers.at(0).UpdateBuffer(context, &gsParticleData, sizeof(gsParticleData));

	std::vector<ID3D11Buffer*> ID3D11Buffers;
	for (ConstantBuffer cb : gsBuffers)
	{
		ID3D11Buffers.push_back(cb.GetBuffer());
	}
	
	ID3D11Buffer** buffersArr = ID3D11Buffers.data();
	context->GSSetConstantBuffers(0, gsBuffers.size(), buffersArr);

	// RS
	D3D11_VIEWPORT viewport = controller.GetViewPort();
	context->RSSetViewports(1, &viewport);

	// PS

	ID3D11PixelShader* pShader = pixelShader.GetPixelShader();
	context->PSSetShader(pShader, nullptr, 0);

	ID3D11DepthStencilView* dsv = camera.GetDepthBuffer().GetDSV();
	ID3D11RenderTargetView* rtv = renderTargetView.GetRTV();
	context->OMSetRenderTargets(1, &rtv, dsv);

	context->Draw(particleBuffer.GetNrOfElements(), 0);

	UnbindPipeline(context);

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

void Renderer::SetupVertexShader(ID3D11DeviceContext* context, ID3D11VertexShader* vertexShader, std::vector<ConstantBuffer> buffers, DX::XMFLOAT4 meshPosition, Camera& camera)
{
	MatrixCreator matrixCreator;

	context->VSSetShader(vertexShader, nullptr, 0);
	
	DX::XMMATRIX worldPositionMatrix = matrixCreator.CreateTranslationMatrix(meshPosition.x, meshPosition.y, meshPosition.z);
	DX::XMFLOAT4X4 worldMatrix;
	DX::XMStoreFloat4x4(&worldMatrix, worldPositionMatrix);

	DX::XMMATRIX viewProjMatrix = matrixCreator.CreateViewProjectionMatrix(camera);
	DX::XMFLOAT4X4 viewProjectionMatrix;
	DX::XMStoreFloat4x4(&viewProjectionMatrix, viewProjMatrix);

	VSGeometryData vsGeometryData;
	vsGeometryData.WorldMatrix = worldMatrix;
	vsGeometryData.ViewProjectionMatrix = viewProjectionMatrix;
	
	buffers.at(0).UpdateBuffer(context, &vsGeometryData, sizeof(vsGeometryData));
	
	std::vector<ID3D11Buffer*> ID3D11Buffers;
	for (ConstantBuffer cb : buffers)
	{
		ID3D11Buffers.push_back(cb.GetBuffer());
	}
	
	ID3D11Buffer** buffersArr = ID3D11Buffers.data();
	context->VSSetConstantBuffers(0, buffers.size(), buffersArr);
}

void Renderer::SetupHullShader(ID3D11DeviceContext* context, ID3D11HullShader* hullShader, std::vector<ConstantBuffer> buffers, const DX::XMFLOAT4& meshPosition, const DX::XMFLOAT4& cameraPosition)
{
	context->HSSetShader(hullShader, nullptr, 0);

	HSGeometryData hullShaderData;
	hullShaderData.CameraPosition = cameraPosition;
	hullShaderData.MeshPosition = meshPosition;

	buffers.at(0).UpdateBuffer(context, &hullShaderData, sizeof(hullShaderData));
	std::vector<ID3D11Buffer*> ID3D11Buffers;
	for (ConstantBuffer cb : buffers)
	{
		ID3D11Buffers.push_back(cb.GetBuffer());
	}
	
	ID3D11Buffer** buffersArr = ID3D11Buffers.data();
	context->HSSetConstantBuffers(0, buffers.size(), buffersArr);
}

void Renderer::SetupDomainShader(ID3D11DeviceContext* context, ID3D11DomainShader* domainShader, std::vector<ConstantBuffer>& buffers, Camera& camera)
{
	context->DSSetShader(domainShader, nullptr, 0);

	MatrixCreator matrixCreator;
	
	DSGeometryData domainShaderData;
	DX::XMFLOAT4X4 viewProjectionMatrix;
	DX::XMMATRIX vpMatrix = matrixCreator.CreateViewProjectionMatrix(camera);
	DX::XMStoreFloat4x4(&viewProjectionMatrix, vpMatrix);

	domainShaderData.ViewProjectionMatrix = viewProjectionMatrix;

	buffers.at(0).UpdateBuffer(context, &domainShaderData, sizeof(domainShaderData));

	std::vector<ID3D11Buffer*> ID3D11Buffers;
	for (ConstantBuffer cb : buffers)
	{
		ID3D11Buffers.push_back(cb.GetBuffer());
	}
	
	ID3D11Buffer** buffersArr = ID3D11Buffers.data();
	context->DSSetConstantBuffers(0, buffers.size(), buffersArr);
}

void Renderer::SetupPixelShader(ID3D11DeviceContext* context, ID3D11PixelShader* pixelShader, ID3D11SamplerState* samplerState, std::vector<ID3D11ShaderResourceView*> textureSRVs, std::vector<ConstantBuffer> buffers, const float& specularExponent)
{
	context->PSSetShader(pixelShader, nullptr, 0);
	context->PSSetSamplers(0, 1, &samplerState);

	ID3D11ShaderResourceView** textures = textureSRVs.data();
	context->PSSetShaderResources(0, textureSRVs.size(), textures);

	PSGeometryData specularExpData;
	specularExpData.SpecularExponent = specularExponent;
	
	buffers.at(0).UpdateBuffer(context, &specularExpData, sizeof(specularExpData));
	
	std::vector<ID3D11Buffer*> ID3D11Buffers;
	for (ConstantBuffer cb : buffers)
	{
		ID3D11Buffers.push_back(cb.GetBuffer());
	}
	
	ID3D11Buffer** buffersArr = ID3D11Buffers.data();
	context->PSSetConstantBuffers(0, buffers.size(), buffersArr);
}

void Renderer::SetupRasterizer(ID3D11DeviceContext* context, ID3D11RasterizerState* rasterizerState, const D3D11_VIEWPORT& viewport)
{
	context->RSSetState(rasterizerState);
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

void Renderer::SetupComputeShader(ID3D11DeviceContext* context, ID3D11ComputeShader* computeShader, ID3D11ShaderResourceView** gBufferSRVs, size_t numGBuffers, ID3D11UnorderedAccessView* uav, std::vector<ConstantBuffer> buffers, Camera& camera, int& outputMode)
{
	context->CSSetShader(computeShader, nullptr, 0);
	context->CSSetShaderResources(0, numGBuffers, gBufferSRVs);
	context->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

	LightConfig lightConfig;
	CSLightData csLightData;

	csLightData.LightColour = lightConfig.GetLightColour();
	csLightData.LightPosition = lightConfig.GetLightPosition();
	csLightData.AmbientLightIntensity = lightConfig.GetAmbientLightIntensity();
	csLightData.GeneralLightIntensity = lightConfig.GetGeneralLightIntensity();
	csLightData.CamPosition = camera.GetPosition();
	csLightData.outputMode = outputMode;
	
	buffers.at(0).UpdateBuffer(context, &csLightData, sizeof(csLightData));

	std::vector<ID3D11Buffer*> ID3D11Buffers;
	for (ConstantBuffer cb : buffers)
	{
		ID3D11Buffers.push_back(cb.GetBuffer());
	}
	
	ID3D11Buffer** buffersArr = ID3D11Buffers.data();
	context->CSSetConstantBuffers(0, buffers.size(), buffersArr);
}

void Renderer::UnbindPipeline(ID3D11DeviceContext* context)
{

	ID3D11Buffer* nullVertexBuffer[1] = { nullptr };
	ID3D11Buffer* nullConstantBuffer[14] = { nullptr };
	ID3D11ShaderResourceView* nullShaderResourceBuffer[128] = { nullptr };
	ID3D11SamplerState* nullSamplerBuffer[16] = { nullptr };
	ID3D11UnorderedAccessView* nullUnorderedAccessViewBuffer[8] = { nullptr };
	ID3D11RenderTargetView* nullRenderTargetViewBuffer[8] = { nullptr };
	D3D11_VIEWPORT nullViewportBuffer[16] = { D3D11_VIEWPORT() };
	
	// IA
	
	UINT stride = 0;
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, nullVertexBuffer, &stride, &offset);
	context->IASetInputLayout(nullptr);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY());

	// VS
	
	context->VSSetShader(nullptr, nullptr, 0);
	context->VSSetConstantBuffers(0, 14, nullConstantBuffer);

	// HS

	context->HSSetShader(nullptr, nullptr, 0);
	context->HSSetConstantBuffers(0, 14, nullConstantBuffer);

	// DS

	context->DSSetShader(nullptr, nullptr, 0);
	context->DSSetConstantBuffers(0, 14, nullConstantBuffer);

	// GS

	context->GSSetShader(nullptr, nullptr, 0);
	context->GSSetConstantBuffers(0, 14, nullConstantBuffer);

	// PS

	context->PSSetShader(nullptr, nullptr, 0);
	context->PSSetSamplers(0, 16, nullSamplerBuffer);
	context->PSSetShaderResources(0, 128, nullShaderResourceBuffer);
	context->PSSetConstantBuffers(0, 14, nullConstantBuffer);

	// Rasterizer

	context->RSSetState(nullptr);
	context->RSSetViewports(16, nullViewportBuffer);

	// OM

	context->OMSetRenderTargets(8, nullRenderTargetViewBuffer, nullptr);

	// CS

	context->CSSetShader(nullptr, nullptr, 0);
	context->CSSetShaderResources(0, 128, nullShaderResourceBuffer);
	context->CSSetUnorderedAccessViews(0, 8, nullUnorderedAccessViewBuffer, nullptr);
	context->CSSetConstantBuffers(0, 14, nullConstantBuffer);
}