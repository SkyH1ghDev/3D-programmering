#include "Setup.hpp"

#include <iostream>
#include <d3dcompiler.h>
#include "WindowHelper.hpp"
#include "D3D11Helper.hpp"
#include "Configuration.hpp"
#include "FileReader.hpp"
#include "PipelineHelper.hpp"
#include "PixelShader.hpp"
#include "VertexShader.hpp"
#include "MatrixCreator.hpp"
#include "PixelShaderData.hpp"

HWND Setup::SetupWindow(HINSTANCE hInstance, int nCmdShow) 
{
	HWND window;
	WindowHelper windowHelper;
	if (!windowHelper.SetupWindow(hInstance, nCmdShow, window))
	{
		std::cerr << "Failed to setup window! \n Exiting... \n";
		exit(-1);	
	}

	return window;
}

D3D11Controller Setup::SetupController(HWND window)
{
    D3D11Helper d3d11Helper;
    Configuration config;

    WindowConfig windowConfig = config.GetWindowConfig();
    UINT height = windowConfig.GetHeight();
    UINT width = windowConfig.GetWidth();

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	D3D11_VIEWPORT viewport;
	
	if (!d3d11Helper.CreateInterfaces(device, immediateContext, swapChain, width, height, window))
	{
		std::cerr << "Failed to setup Device, ImmediateContext and SwapChain \n Exiting... \n";
		device->Release();
		immediateContext->Release();
		swapChain->Release();
		exit(-1);
	}
	
    d3d11Helper.SetViewport(viewport, width, height);

	return D3D11Controller(device, immediateContext, swapChain, viewport);
}

RenderTarget Setup::SetupRenderTarget(D3D11Controller &controller)
{
    D3D11Helper d3d11Helper;

	ID3D11RenderTargetView* rtv;
	if (!d3d11Helper.CreateRenderTargetView(controller.GetDevice(), controller.GetSwapChain(), rtv))
	{
		std::cerr << "Failed to setup Render Target View \n Exiting... \n";
		rtv->Release();
		exit(-1);
	}

	Configuration configuration;
	WindowConfig windowConfig = configuration.GetWindowConfig();
	ID3D11Texture2D* texture;
	ID3D11DepthStencilView* dsv;
	if (!d3d11Helper.CreateDepthStencil(controller.GetDevice(), windowConfig.GetWidth(), windowConfig.GetHeight(), texture, dsv))
	{
		std::cerr << "Failed to setup Depth Stencil View \n Exiting... \n";
		texture->Release();
		dsv->Release();
		exit(-1);
	}

	return RenderTarget(rtv, texture, dsv);
}

Scene Setup::SetupScene(D3D11Controller &controller)
{
	
	std::vector<MeshData> meshDataList;
	FileReader fileReader;
	if (fileReader.ReadFilesFromConfig(meshDataList) == -1)
	{
		std::cerr << "Failed to Read OBJ-Files \n Exiting... \n";
		exit(-1);
	}

	HRESULT hr;

	Scene scene(hr, controller.GetDevice());
	if (FAILED(hr))
	{
		std::cerr << "Failed to create Scene \n Exiting... \n";
		exit(-1);
	}
	
	for (MeshData meshData : meshDataList)
	{
		Mesh mesh = Mesh(hr, controller.GetDevice(), meshData);

		if(FAILED(hr))
		{
			std::cerr << "Failed to create Mesh \n Exiting... \n";
			exit(-1);
		}
		
		scene.AddMesh(mesh);
	}

	return scene;
}

Shader* Setup::SetupShader(D3D11Controller& controller, ShaderType shaderType, LPCWSTR csoPath)
{
	PipelineHelper pipelineHelper;
	ID3DBlob* shaderBlob;

	if (!pipelineHelper.LoadShaderBlob(shaderBlob, shaderType, csoPath))
	{
		std::cerr << "Failed to Read Shader Data \n Exiting... \n";
		shaderBlob->Release();
		exit(-1);
	}
	
	switch (shaderType)
	{
		case ShaderType::VERTEX_SHADER:
			ID3D11VertexShader* vertexShader;
		
			if (!pipelineHelper.LoadVertexShader(controller.GetDevice(), vertexShader, shaderBlob))
			{
				std::cerr << "Could not Compile Vertex Shader \n Exiting... \n";
				vertexShader->Release();
				shaderBlob->Release();
				exit(-1);
			}

			return new VertexShader(vertexShader, shaderBlob);
		
		case ShaderType::HULL_SHADER:

			// TODO: Fix loading of hull shader
			
			std::cerr << "Not implemented loading (HS) \n Exiting... \n";
			exit(-1);
			break;
		
		case ShaderType::DOMAIN_SHADER:
			
			// TODO: Fix loading of domain shader
				
			std::cerr << "Not implemented loading (DS) \n Exiting... \n";
			exit(-1);
			break;

		case ShaderType::GEOMETRY_SHADER:
			
			// TODO: Fix loading of geometry shader
				
			std::cerr << "Not implemented loading (GS) \n Exiting... \n";
			exit(-1);
			break;

		case ShaderType::PIXEL_SHADER:
			ID3D11PixelShader* pixelShader;

			if (!pipelineHelper.LoadPixelShader(controller.GetDevice(), pixelShader, shaderBlob))
			{
				std::cerr << "Could not Compile Pixel Shader \n Exiting... \n";
				pixelShader->Release();
				shaderBlob->Release();
				exit(-1);
			}

			return new PixelShader(pixelShader, shaderBlob);

		case ShaderType::COMPUTE_SHADER:
			
			// TODO: Fix loading of compute shader

			std::cerr << "Not implemented loading (CS) \n Exiting... \n";
			exit(-1);
			break;
	}
}

InputLayout Setup::SetupInputLayout(D3D11Controller &controller, const Shader &vertexShader)
{
	PipelineHelper pipelineHelper;
	ID3D11InputLayout* inputLayout;

	ID3DBlob* shaderBlob = vertexShader.GetShaderBlob();

	pipelineHelper.CreateInputLayout(controller.GetDevice(), inputLayout, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize());

	return InputLayout(inputLayout);
}

Sampler Setup::SetupSampler(D3D11Controller &controller)
{
	PipelineHelper pipelineHelper;
	ID3D11SamplerState* sampler;

	pipelineHelper.CreateSamplerState(controller.GetDevice(), sampler);

	return Sampler(sampler);
}

ConstantBuffer Setup::CreateWorldMatrixConstantBuffer(D3D11Controller &controller)
{
    HRESULT hr;
    
    MatrixCreator matrixCreator;

    BufferFlagData worldMatrixBufferFlagData;
    worldMatrixBufferFlagData.Usage = D3D11_USAGE_DYNAMIC;
    worldMatrixBufferFlagData.CpuAccess = D3D11_CPU_ACCESS_WRITE;
   
	DX::XMFLOAT4X4 worldMatrixFloat4x4 = matrixCreator.CreateWorldXMFLOAT4X4();
	ConstantBuffer cbWorldMatrix = ConstantBuffer(hr, controller.GetDevice(), sizeof(worldMatrixFloat4x4),
	                                                &worldMatrixFloat4x4, 0, 0, 0,
	                                                worldMatrixBufferFlagData);

	if (FAILED(hr))
	{
		std::cerr << "Create worldMatrixBuffer Failed" << std::endl;
		exit(-1);
	}

	return cbWorldMatrix;
}

ConstantBuffer Setup::CreatePixelShaderConstantBuffer(D3D11Controller &controller, Scene& scene)
{
	HRESULT hr;
	
	PixelShaderData psData;
	
	psData.LightColour = {1.0f, 1.0f, 1.0f, 1.0f};
	psData.LightPosition = {0.0f, 1.0f, -10.0f, 1.0f};
	psData.EyePosition = DX::XMFLOAT4(scene.GetCurrentCamera().GetPosition());
	psData.AmbientLightIntensity = 0.1f;
	psData.Shininess = 10000.0f;

	BufferFlagData psBufferFlags;
	psBufferFlags.Usage = D3D11_USAGE_IMMUTABLE;
	
	ConstantBuffer psConstBuffer = ConstantBuffer(hr, controller.GetDevice(), sizeof(psData), &psData, 0, 0, 0, psBufferFlags);
    
	if (FAILED(hr))
	{
		std::cerr << "Create psBuffer Failed" << std::endl;
		exit(-1);
	}

	return psConstBuffer;
}

