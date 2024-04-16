#define STB_IMAGE_IMPLEMENTATION
#define DX DirectX

#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include <chrono>

#include "Configuration.hpp"
#include "ConstantBuffer.hpp"
#include "WindowHelper.h"
#include "D3D11Helper.hpp"
#include "PipelineHelper.hpp"
#include "DirectXMath.h"
#include "stb_image.h"
#include "FileReader.hpp"
#include "ManagerHelper.hpp"
#include "MatrixCreator.hpp"
#include "Renderer.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
	Configuration setupConfiguration;
	
	FileReader fileReader;
	FileConfig fileConfig = setupConfiguration.GetFileConfig();
	fileReader.ReadFilesFromConfig(fileConfig);

	HWND window;
	WindowConfig windowConfig = setupConfiguration.GetWindowConfig();
	WindowHelper windowHelper;
	if (!windowHelper.SetupWindow(hInstance, windowConfig.GetWidth(), windowConfig.GetHeight(), nCmdShow, window))
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


	D3D11Helper d3d11Helper;
	if (!d3d11Helper.SetupD3D11(windowConfig.GetWidth(), windowConfig.GetHeight(), window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Failed to setup d3d11!" << std::endl;
		return -1;
	}

	PipelineHelper pipelineHelper;
	if (!pipelineHelper.SetupPipeline(device, vertexBuffer, vShader, pShader, inputLayout, texture, textureSRV, samplerState, imageData))
	{
		std::cerr << "Failed to setup pipeline!" << std::endl;
		return -1;
	}
	
   // Create World Matrix
	MatrixCreator matrixCreator;
	WorldMatrixConfig worldMatrixConfig;
	DX::XMMATRIX worldMatrix = matrixCreator.CreateWorldMatrix(worldMatrixConfig);
	
	// Create View Matrix
	ViewMatrixConfig viewMatrixConfig;
	DX::XMMATRIX viewMatrix = matrixCreator.CreateViewMatrix(viewMatrixConfig);
	
	// Create Projection Matrix
	ProjectionMatrixConfig projectionMatrixConfig;
	DX::XMMATRIX projectionMatrix = matrixCreator.CreateProjectionMatrix(projectionMatrixConfig);
	
	// Combine View + Projection Matrices And Transpose The Result

	DX::XMMATRIX viewProjectionMatrix = XMMatrixMultiplyTranspose(viewMatrix, projectionMatrix);

	// Convert Matrices into XMFloat4x4

	DX::XMFLOAT4X4 worldMatrixFloat4x4;
	XMStoreFloat4x4(&worldMatrixFloat4x4, worldMatrix);

	DX::XMFLOAT4X4 viewProjectionMatrixFloat4x4;
	XMStoreFloat4x4(&viewProjectionMatrixFloat4x4, viewProjectionMatrix);

	// Create Constant Buffer Of Matrices For VS
	
	std::vector<DX::XMFLOAT4X4> matrixVector = {worldMatrixFloat4x4, viewProjectionMatrixFloat4x4};

	HRESULT hr;

	BufferFlagData vsBufferFlags;
	vsBufferFlags.Usage = D3D11_USAGE_DYNAMIC;
	vsBufferFlags.CpuAccess = D3D11_CPU_ACCESS_WRITE;
	
	
	ConstantBuffer vsConstBuffer = ConstantBuffer(hr, device, sizeof(worldMatrixFloat4x4) + sizeof(viewProjectionMatrixFloat4x4), matrixVector.data(), 0, 0, 0, vsBufferFlags);
	
	if (FAILED(hr))
	{
		std::cerr << "Create vsBuffer Failed" << std::endl;
		return -1;
	}
	
	// Create Constant Buffer For PS
	struct psStruct
	{
		DX::XMFLOAT4 lightColour;
		DX::XMFLOAT4 lightPosition; 
		DX::XMFLOAT4 eyePosition;
		float ambientLightIntensity;
		float shininess;
		char padding[8];
	};

	psStruct psValues;
	psValues.lightColour = {1.0f, 1.0f, 1.0f, 1.0f};
	psValues.lightPosition = {0.0f, 1.0f, -10.0f, 1.0f};
	psValues.eyePosition = DX::XMFLOAT4(viewMatrixConfig.GetEyePosition().data());
	psValues.ambientLightIntensity = 0.1f;
	psValues.shininess = 10000.0f;

	BufferFlagData psBufferFlags;
	psBufferFlags.Usage = D3D11_USAGE_IMMUTABLE;
	
	ConstantBuffer psConstBuffer = ConstantBuffer(hr, device, sizeof(psStruct), &psValues, 0, 0, 0, psBufferFlags);

	if (FAILED(hr))
	{
		std::cerr << "Create psBuffer Failed" << std::endl;
		return -1;
	}

	ID3D11Buffer* vertexShaderConstantBuffer = vsConstBuffer.GetBuffer();
	immediateContext->VSSetConstantBuffers(0, 1, &vertexShaderConstantBuffer);

	ID3D11Buffer* pixelShaderConstBuffer = psConstBuffer.GetBuffer();
	immediateContext->PSSetConstantBuffers(0, 1, &pixelShaderConstBuffer);

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

	Renderer renderer;
	float rotationalSpeed = 1.0f;
	float currentAngle = worldMatrixConfig.GetInitialAngle();
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
		
		renderer.Render(immediateContext, rtv, dsView, viewport, vShader, pShader, inputLayout, vertexBuffer, textureSRV, samplerState);
		swapChain->Present(0, 0);

		// Map New World Matrix Every Frame
		
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(mappedResource));

		immediateContext->Map(vertexShaderConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		XMStoreFloat4x4(&worldMatrixFloat4x4, matrixCreator.CreateWorldMatrix(currentAngle));
		matrixVector[0] = worldMatrixFloat4x4;
		memcpy(mappedResource.pData, matrixVector.data(), sizeof(matrixVector.at(0)) * matrixVector.size());
		immediateContext->Unmap(vertexShaderConstantBuffer, 0);

		time_point<high_resolution_clock> t2 = high_resolution_clock::now();
		duration<float> timeDiff = t1 - t2;
		float deltaTime = timeDiff.count();

		currentAngle -= rotationalSpeed * deltaTime;
	}

	rasterizerState->Release();
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
