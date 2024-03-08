#define STB_IMAGE_IMPLEMENTATION

#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include <chrono>

#include "WindowHelper.h"
#include "D3D11Helper.h"
#include "PipelineHelper.h"
#include "DirectXMath.h"
#include "stb_image.h"
#include "FileReader.hpp"
#include "ManagerHelper.hpp"
#include "VertexManager.hpp"

void Render(ID3D11DeviceContext* immediateContext, ID3D11RenderTargetView* rtv,
            ID3D11DepthStencilView* dsView, D3D11_VIEWPORT& viewport, ID3D11VertexShader* vShader,
            ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout, ID3D11Buffer* vertexBuffer, ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState* samplerState)
{
	float clearColour[4] = { 0, 0, 0, 0 };
	VertexManager* vertexManagerInstance = VertexManager::GetInstance();
	
	immediateContext->ClearRenderTargetView(rtv, clearColour);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShaderResources(0, 1, &textureSRV);
	immediateContext->PSSetSamplers(0, 1, &samplerState);
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);
	immediateContext->Draw(vertexManagerInstance->numVerticesToDraw, 0);
}

DirectX::XMMATRIX CreateWorldMatrix(float &angle)
{
	using namespace DirectX;
	XMMATRIX translationMatrix = XMMatrixTranslation(0, 0, 0);
	XMMATRIX rotationMatrix = XMMatrixRotationY(angle);
	return XMMatrixTranspose(XMMatrixMultiply(translationMatrix, rotationMatrix));
}

DirectX::XMMATRIX CreateViewMatrix(const DirectX::XMFLOAT4* eyePos)
{
	using namespace DirectX;
	XMVECTOR eyePosition = XMLoadFloat4(eyePos);
	XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	return XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}

DirectX::XMMATRIX CreateProjectionMatrix(const float &fovAngle, const float &aspectRatio, const float &nearZ, const float &farZ)
{
	using namespace DirectX;
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngle), aspectRatio, nearZ, farZ);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	FileReader fileReader;
	if (fileReader.ReadFile("lego.obj") == -1)
	{
		return -1;
	}

	using namespace DirectX;
	
	const UINT WIDTH = 1280;
	const UINT HEIGHT = 720;
	HWND window;

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Failed to setup window!" << std::endl;
		return -1;
	}

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	D3D11_VIEWPORT viewport;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;
	unsigned char* imageData;
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureSRV;
	ID3D11SamplerState* samplerState;

	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Failed to setup d3d11!" << std::endl;
		return -1;
	}

	if (!SetupPipeline(device, vertexBuffer, vShader, pShader, inputLayout, texture, textureSRV, samplerState, imageData))
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return -1;
	}

	// Create World Matrix
	float initialAngle = 0.0f;
	XMMATRIX worldMatrix = CreateWorldMatrix(initialAngle);

	// Create View Matrix
	XMFLOAT4 eyePos = {0.0f, 0.0f, -15.0f, 1.0f};
	XMMATRIX viewMatrix = CreateViewMatrix(&eyePos);

	// Create Projection Matrix
	const float fovAngle = 59.0f; // Degrees
	const float aspectRatio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
	const float nearZ = 0.1f;
	const float farZ = 100.0f;
	XMMATRIX projectionMatrix = CreateProjectionMatrix(fovAngle, aspectRatio, nearZ, farZ);

	// Combine View + Projection Matrices And Transpose The Result

	XMMATRIX viewProjectionMatrix = XMMatrixMultiplyTranspose(viewMatrix, projectionMatrix);

	// Convert Matrices into XMFloat4x4

	XMFLOAT4X4 worldMatrixFloat4x4;
	XMStoreFloat4x4(&worldMatrixFloat4x4, worldMatrix);

	XMFLOAT4X4 viewProjectionMatrixFloat4x4;
	XMStoreFloat4x4(&viewProjectionMatrixFloat4x4, viewProjectionMatrix);

	// Create Constant Buffer Of Matrices For VS
	
	XMFLOAT4X4 matrixArr[] = {worldMatrixFloat4x4, viewProjectionMatrixFloat4x4};

	D3D11_BUFFER_DESC vsConstBuffer;
	vsConstBuffer.ByteWidth = sizeof(worldMatrixFloat4x4) + sizeof(viewProjectionMatrixFloat4x4);
	vsConstBuffer.Usage = D3D11_USAGE_DYNAMIC;
	vsConstBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vsConstBuffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vsConstBuffer.MiscFlags = 0;
	vsConstBuffer.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vsConstBufferSubResource;
	vsConstBufferSubResource.pSysMem = matrixArr;
	vsConstBufferSubResource.SysMemPitch = 0;
	vsConstBufferSubResource.SysMemSlicePitch = 0;

	// Checks if byteWidth is a multiple of 16
	static_assert(sizeof(matrixArr) % 16 == 0);
	
	ID3D11Buffer* vertexShaderConstantBuffer;
	HRESULT hr = device->CreateBuffer(&vsConstBuffer, &vsConstBufferSubResource, &vertexShaderConstantBuffer);
	
	if (FAILED(hr))
	{
		std::cerr << "Create vsBuffer Failed" << std::endl;
		return -1;
	}
	
	// Create Constant Buffer For PS
	struct psStruct
	{
		XMFLOAT4 lightColour;
		XMFLOAT4 lightPosition; 
		XMFLOAT4 eyePosition;
		float ambientLightIntensity;
		float shininess;
		char padding[8];
	};

	psStruct psValues;
	psValues.lightColour = {1.0f, 1.0f, 1.0f, 1.0f};
	psValues.lightPosition = {0.0f, 1.0f, -10.0f, 1.0f};
	psValues.eyePosition = eyePos;
	psValues.ambientLightIntensity = 0.1f;
	psValues.shininess = 10000.0f;
	
	D3D11_BUFFER_DESC psConstBuffer;
	psConstBuffer.ByteWidth = sizeof(psStruct);
	psConstBuffer.Usage = D3D11_USAGE_IMMUTABLE;
	psConstBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	psConstBuffer.CPUAccessFlags = 0;
	psConstBuffer.MiscFlags = 0;
	psConstBuffer.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA psConstBufferSubResource;
	psConstBufferSubResource.pSysMem = &psValues;
	psConstBufferSubResource.SysMemPitch = 0;
	psConstBufferSubResource.SysMemSlicePitch = 0;

	// Check if byteWidth is a multiple of 16
	static_assert(sizeof(psStruct) % 16 == 0);

	ID3D11Buffer* pixelShaderConstantBuffer;
	hr = device->CreateBuffer(&psConstBuffer, &psConstBufferSubResource, &pixelShaderConstantBuffer);

	if (FAILED(hr))
	{
		std::cerr << "Create psBuffer Failed" << std::endl;
		return -1;
	}

	immediateContext->VSSetConstantBuffers(0, 1, &vertexShaderConstantBuffer);
	immediateContext->PSSetConstantBuffers(0, 1, &pixelShaderConstantBuffer);

	// DEBUG RASTERIZER DESC
	
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = false;
	

	ID3D11RasterizerState* rasterizerState;
	hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

	if (FAILED(hr))
	{
		std::cerr << "Create RasterizerState failed" << std::endl;
		return -1;
	}

	immediateContext->RSSetState(rasterizerState); 

	// DEBUG RASTERIZER DESC END
	
	float rotationalSpeed = 1.0f;
	MSG msg = { };
	while (!(GetKeyState(VK_ESCAPE) & 0b1000000000000000) && msg.message != WM_QUIT)
	{
		using namespace std::chrono;
		time_point<high_resolution_clock> t1 = high_resolution_clock::now();
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		Render(immediateContext, rtv, dsView, viewport, vShader, pShader, inputLayout, vertexBuffer, textureSRV, samplerState);
		swapChain->Present(0, 0);

		// Map New World Matrix Every Frame
		
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(mappedResource));

		immediateContext->Map(vertexShaderConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		XMStoreFloat4x4(&worldMatrixFloat4x4, CreateWorldMatrix(initialAngle));
		matrixArr[0] = worldMatrixFloat4x4;
		memcpy(mappedResource.pData, matrixArr, sizeof(matrixArr));
		immediateContext->Unmap(vertexShaderConstantBuffer, 0);

		time_point<high_resolution_clock> t2 = high_resolution_clock::now();
		duration<float> timeDiff = t1 - t2;
		float deltaTime = timeDiff.count();

		initialAngle -= rotationalSpeed * deltaTime;
	}

	rasterizerState->Release();
	pixelShaderConstantBuffer->Release();
	vertexShaderConstantBuffer->Release();
	samplerState->Release();
	textureSRV->Release();
	texture->Release();
	stbi_image_free(imageData);
	vertexBuffer->Release();
	inputLayout->Release();
	pShader->Release();
	vShader->Release();
	dsView->Release();
	dsTexture->Release();
	rtv->Release();
	swapChain->Release();
	immediateContext->Release();
	device->Release();

	ManagerHelper::ReleaseManagers();

	return 0;
}