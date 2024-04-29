#define STB_IMAGE_IMPLEMENTATION

#include <Windows.h>
#include <iostream>
#include <d3d11.h>

#include "Configuration.hpp"
#include "ConstantBuffer.hpp"
#include "WindowHelper.hpp"
#include "D3D11Helper.hpp"
#include "PipelineHelper.hpp"
#include "DirectXMath.h"
#include "stb_image.h"
#include "FileReader.hpp"
#include "ManagerHelper.hpp"
#include "MatrixCreator.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Clock.hpp"
#include "Input.hpp"

namespace DX = DirectX;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
	
	FileReader fileReader;
	fileReader.ReadFilesFromConfig();

	HWND window;
	WindowHelper windowHelper;
	if (!windowHelper.SetupWindow(hInstance, nCmdShow, window))
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
	if (!d3d11Helper.SetupD3D11(window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport))
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

	HRESULT hr;
	Camera mainCam(hr, device);
	
	MatrixCreator matrixCreator;
	
	BufferFlagData worldMatrixCBFlags;
	worldMatrixCBFlags.Usage = D3D11_USAGE_DYNAMIC;
	worldMatrixCBFlags.CpuAccess = D3D11_CPU_ACCESS_WRITE;
	
	DX::XMFLOAT4X4 worldMatrixFloat4x4 = matrixCreator.CreateWorldXMFLOAT4X4();
	ConstantBuffer cbWorldMatrix = ConstantBuffer(hr, device, sizeof(worldMatrixFloat4x4), &worldMatrixFloat4x4, 0, 0, 0, worldMatrixCBFlags);
	
	if (FAILED(hr))
	{
		std::cerr << "Create worldMatrixBuffer Failed" << std::endl;
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
	psValues.eyePosition = DX::XMFLOAT4(mainCam.GetPosition());
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

	ID3D11Buffer* worldMatrixConstBuffer = cbWorldMatrix.GetBuffer();
	ID3D11Buffer* viewProjectionMatrixConstBuffer = mainCam.GetConstantBuffer();
	immediateContext->VSSetConstantBuffers(0, 1, &worldMatrixConstBuffer);
	immediateContext->VSSetConstantBuffers(1, 1, &viewProjectionMatrixConstBuffer);

	ID3D11Buffer* pixelShaderConstBuffer = psConstBuffer.GetBuffer();
	immediateContext->PSSetConstantBuffers(0, 1, &pixelShaderConstBuffer);

	Input input;
	Clock clock;
	Renderer renderer;
	float rotationalSpeed = 1.0f;
	WorldMatrixConfig worldMatrixConfig;
	float currentAngle = worldMatrixConfig.GetInitialAngle();
	MSG msg = { };
	while (input.Exit(msg))
	{
		clock.Start();
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		renderer.Render(immediateContext, rtv, dsView, viewport, vShader, pShader, inputLayout, vertexBuffer, textureSRV, samplerState);
		swapChain->Present(0, 0);
		
		mainCam.UpdateInternalConstantBuffer(immediateContext);
		
		worldMatrixFloat4x4 = matrixCreator.CreateWorldXMFLOAT4X4(currentAngle);
		cbWorldMatrix.UpdateBuffer(immediateContext, &worldMatrixFloat4x4, sizeof(worldMatrixFloat4x4));
		
		clock.End();
		float deltaTime = clock.GetDeltaTime();

		input.ReadInput(mainCam, deltaTime);
		currentAngle -= rotationalSpeed * deltaTime;
	}

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
