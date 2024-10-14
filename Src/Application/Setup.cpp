#include "Setup.hpp"

#include <codecvt>
#include <iostream>
#include <d3dcompiler.h>

#include "ComputeShader.hpp"
#include "WindowHelper.hpp"
#include "D3D11Helper.hpp"
#include "Configuration.hpp"
#include "DomainShader.hpp"
#include "FileReader.hpp"
#include "GeometryShader.hpp"
#include "HullShader.hpp"
#include "PipelineHelper.hpp"
#include "PixelShader.hpp"
#include "VertexShader.hpp"
#include "MatrixCreator.hpp"
#include "LightData.hpp"

HWND Setup::SetupWindow(HINSTANCE hInstance, int nCmdShow) 
{
	HWND window;
	WindowHelper windowHelper;
	if (!windowHelper.SetupWindow(hInstance, nCmdShow, window))
	{
		throw std::runtime_error("Failed to setup Window");
	}

	return window;
}

Controller Setup::SetupController()
{
    D3D11Helper d3d11Helper;

    WindowConfig windowConfig;
    UINT height = windowConfig.GetHeight();
    UINT width = windowConfig.GetWidth();

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	D3D11_VIEWPORT viewport;
	
	if (!d3d11Helper.CreateController(device, immediateContext))
	{
		device->Release();
		immediateContext->Release();
		throw std::runtime_error("Failed to setup Device, Context and Swap Chain");
	}
	
    d3d11Helper.SetViewport(viewport, width, height);

	return Controller(device, immediateContext, viewport);
}

SwapChain Setup::SetupSwapChain(Controller& controller, HWND window)
{
	WindowConfig windowConfig;
	UINT width = windowConfig.GetWidth();
	UINT height = windowConfig.GetHeight();
	
	D3D11Helper d3d11Helper;
	
	IDXGISwapChain* swapChain;
	ID3D11Texture2D* backBuffer;
	ID3D11UnorderedAccessView* uav;
	
	d3d11Helper.CreateSwapChain(controller.GetDevice(), swapChain, backBuffer, uav, window, width, height);
	
	return SwapChain(swapChain, backBuffer, uav);
}


RenderTargetView Setup::SetupRenderTargetView(Controller &controller, SwapChain& swapChain)
{
    D3D11Helper d3d11Helper;

	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* backBuffer = swapChain.GetBackBuffer();
	if (!d3d11Helper.CreateRenderTargetView(controller.GetDevice(), backBuffer, rtv))
	{
		rtv->Release();
		throw std::runtime_error("Failed to create RTV");
	}

	return RenderTargetView(rtv);
}

RenderTargetView Setup::SetupGBuffer(Controller& controller)
{
	D3D11Helper d3d11Helper;
	WindowConfig windowConfig;

	
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
	ID3D11Texture2D* texture = nullptr;
	if (!d3d11Helper.CreateGBuffer(controller.GetDevice(), rtv, texture, srv, windowConfig.GetWidth(), windowConfig.GetHeight()))
	{
		if (rtv != nullptr) { rtv->Release(); }
		if (texture != nullptr) { texture->Release(); }
		if (srv != nullptr) { srv->Release(); }
		throw std::runtime_error("Failed to create GBuffer");
	}

	return RenderTargetView(rtv, texture, srv);
}


Scene Setup::SetupScene(Controller &controller)
{
	std::vector<MeshData> meshDataList;
	FileReader fileReader;
	
	if (fileReader.ReadFilesFromConfig(meshDataList) == -1)
	{
		throw std::runtime_error("Failed to Read OBJ-Files");
	}

	HRESULT hr = 0;

	ViewMatrixConfig viewMatrixConfig;
	ProjectionMatrixConfig projectionMatrixConfig;
	ProjectionInfo projectionInfo;

	projectionInfo.AspectRatio = projectionMatrixConfig.GetAspectRatio();
	projectionInfo.FarZ = projectionMatrixConfig.GetFarZ();
	projectionInfo.NearZ = projectionMatrixConfig.GetNearZ();
	projectionInfo.FovAngleY = projectionMatrixConfig.GetFovAngle();

	std::vector<Camera> cameraVector;

	DX::XMFLOAT4 mainCamPosition = viewMatrixConfig.GetCamPositionAt(0);
	
	cameraVector.push_back(Camera(hr, controller.GetDevice(), projectionInfo, mainCamPosition));

	DX::XMFLOAT4 secondaryCamPosition = viewMatrixConfig.GetCamPositionAt(1);
	cameraVector.push_back(Camera(hr, controller.GetDevice(), projectionInfo, secondaryCamPosition));
	
	Scene scene(cameraVector);
	
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to Create Scene");
	}
	
	for (MeshData meshData : meshDataList)
	{
		Mesh mesh = Mesh(hr, controller.GetDevice(), meshData);

		if(FAILED(hr))
		{
			throw std::runtime_error("Failed to create Mesh");
		}
		
		scene.AddMesh(mesh);
	}

	return scene;
}

Shader* Setup::SetupShader(Controller& controller, ShaderType shaderType, std::wstring hlslFilename)
{
	PipelineHelper pipelineHelper;
	ID3DBlob* shaderBlob;

	union Shader
	{
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11HullShader* hullShader;
		ID3D11DomainShader* domainShader;
		ID3D11GeometryShader* geometryShader;
		ID3D11PixelShader* pixelShader;
		ID3D11ComputeShader* computeShader;
	};

	Shader shader;
	
	std::wstring hlslFilepath = L"Src/Shaders/" + hlslFilename;
	
	if (!pipelineHelper.LoadShaderBlob(shaderBlob, shaderType, hlslFilepath.c_str()))
	{
		throw std::runtime_error("Failed to Read Shader Data");
	}
	
	switch (shaderType)
	{
		case ShaderType::VERTEX_SHADER:
			if (!pipelineHelper.LoadVertexShader(controller.GetDevice(), shader.vertexShader, shaderBlob))
			{
				std::cerr << "Could not Compile Vertex Shader \n Exiting... \n";
				shader.vertexShader->Release();
				shaderBlob->Release();
				throw std::runtime_error("Could not Compile Vertex Shader");
			}

			return new VertexShader(shader.vertexShader, shaderBlob);
		
		case ShaderType::HULL_SHADER:
			if (!pipelineHelper.LoadHullShader(controller.GetDevice(), shader.hullShader, shaderBlob))
			{
				shader.domainShader->Release();
				shaderBlob->Release();
				throw std::runtime_error("Could not Compile Hull Shader");
			}
		
			return new HullShader(shader.hullShader, shaderBlob);
		
		case ShaderType::DOMAIN_SHADER:
			if (!pipelineHelper.LoadDomainShader(controller.GetDevice(), shader.domainShader, shaderBlob))
			{
				shader.domainShader->Release();
				shaderBlob->Release();
				throw std::runtime_error("Could not Compile Domain Shader");
			}
		
			return new DomainShader(shader.domainShader, shaderBlob);
			
		case ShaderType::GEOMETRY_SHADER:
			if (!pipelineHelper.LoadGeometryShader(controller.GetDevice(), shader.geometryShader, shaderBlob))
			{
				shader.geometryShader->Release();
				shaderBlob->Release();
				throw std::runtime_error("Could not Compile Geometry Shader");
			}
		
			return new GeometryShader(shader.geometryShader, shaderBlob);
		
		case ShaderType::PIXEL_SHADER:
			if (!pipelineHelper.LoadPixelShader(controller.GetDevice(), shader.pixelShader, shaderBlob))
			{
				shader.pixelShader->Release();
				shaderBlob->Release();
				throw std::runtime_error("Could not Compile Pixel Shader");
			}

			return new PixelShader(shader.pixelShader, shaderBlob);

		case ShaderType::COMPUTE_SHADER:
			if (!pipelineHelper.LoadComputeShader(controller.GetDevice(), shader.computeShader, shaderBlob))
			{
				shader.computeShader->Release();
				shaderBlob->Release();
				throw std::runtime_error("Could not Compile Compute Shader");
			}
			return new ComputeShader(shader.computeShader, shaderBlob);

		default:
			throw std::runtime_error("How did you get here?");
	}
}

Rasterizer Setup::SetupRasterizer(Controller& controller)
{
	PipelineHelper pipelineHelper;
	ID3D11RasterizerState* rasterizerState;

	if (FAILED(pipelineHelper.CreateRasterizer(controller.GetDevice(), rasterizerState)))
	{
		throw std::runtime_error("Failed to create Rasterizer");
	}

	return Rasterizer(rasterizerState);
}


InputLayout Setup::SetupInputLayout(Controller &controller, const Shader &vertexShader)
{
	PipelineHelper pipelineHelper;
	ID3D11InputLayout* inputLayout;

	ID3DBlob* shaderBlob = vertexShader.GetShaderBlob();

	if (FAILED(pipelineHelper.CreateInputLayout(controller.GetDevice(), inputLayout, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize())))
	{
		throw std::runtime_error("Failed to create Input Layout");
	}

	return InputLayout(inputLayout);
}

Sampler Setup::SetupSampler(Controller &controller)
{
	PipelineHelper pipelineHelper;
	ID3D11SamplerState* sampler;

	if (FAILED(pipelineHelper.CreateSamplerState(controller.GetDevice(), sampler)))
	{
		throw std::runtime_error("Failed to Create Sampler State");
	}

	return Sampler(sampler);
}