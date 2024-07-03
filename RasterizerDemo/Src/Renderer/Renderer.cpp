#include "Renderer.hpp"
#include "VertexManager.hpp"

void Renderer::Render(D3D11Controller &controller, RenderTarget &rtv, VertexShader &vertexShader, PixelShader &pixelShader, InputLayout &inputLayout, Mesh& mesh, Sampler& samplerState)
{
	ID3D11DeviceContext* context = controller.GetContext();
	ID3D11RenderTargetView* rTargetView = rtv.GetRTV();
	ID3D11DepthStencilView* dStencilView = rtv.GetDSV();
	
	ClearScreen(context, rTargetView, dStencilView);

	VertexBuffer vertexBuffer = mesh.GetVertexBuffer();
	
	ID3D11Buffer* vBuffer = vertexBuffer.GetBuffer();
	ID3D11InputLayout* iLayout = inputLayout.GetInputLayout();
	
	SetupInputAssembler(context, vBuffer, iLayout, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11VertexShader* vShader = vertexShader.GetVertexShader();

	SetupVertexShader(context, vShader);

	ID3D11PixelShader* pShader = pixelShader.GetPixelShader();
	ID3D11ShaderResourceView* tShaderResourceView = mesh.GetTextureSRV(0); // INDEX IS JUST FOR TESTING PURPOSES
	ID3D11SamplerState* sState = samplerState.GetSamplerState();
	
	SetupPixelShader(context, pShader, tShaderResourceView, sState);

	D3D11_VIEWPORT viewport = controller.GetViewPort();
	
	SetupRasterizer(context, viewport);

	SetupOutputMerger(context, rTargetView, dStencilView);

	context->Draw(vertexBuffer.GetNrOfVertices(), 0);
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

void Renderer::SetupOutputMerger(ID3D11DeviceContext* context, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
	context->OMSetRenderTargets(1, &rtv, dsv);
}

