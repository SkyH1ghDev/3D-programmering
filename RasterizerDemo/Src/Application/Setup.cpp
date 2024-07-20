#include "Setup.hpp"

#include <codecvt>
#include <iostream>
#include <d3dcompiler.h>

#include "ComputeShader.hpp"
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
		throw std::runtime_error("Failed to setup Window");
	}

	return window;
}

Controller Setup::SetupController(HWND window)
{
    D3D11Helper d3d11Helper;
    Configuration config;

    WindowConfig windowConfig = config.GetWindowConfig();
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
	Configuration config;

	WindowConfig windowConfig = config.GetWindowConfig();
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
	Configuration config;

	WindowConfig windowConfig = config.GetWindowConfig();

	
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

	HRESULT hr;

	Scene scene(hr, controller.GetDevice());
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
		
	}
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

/*RenderTargetView Setup::SetupGBuffer(D3D11Controller& controller)
{
	WindowConfig windowConfig;
	
	D3D11_TEXTURE2D_DESC texture2DDesc;
	texture2DDesc.Width = windowConfig.GetWidth();
	texture2DDesc.Height = windowConfig.GetHeight();
	texture2DDesc.MipLevels = 1;
	texture2DDesc.ArraySize = 1;
	texture2DDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texture2DDesc.SampleDesc.Count = 1;
	texture2DDesc.SampleDesc.Quality = 0;
	texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texture2DDesc.CPUAccessFlags = 0;
	texture2DDesc.MiscFlags = 0;

	ID3D11Device* device = controller.GetDevice();
	
	ID3D11Texture2D* texture = nullptr;
	if (FAILED(device->CreateTexture2D(&texture2DDesc, nullptr, &texture)))
	{
		throw std::runtime_error("Failed to create G-Buffer Texture");
	}

	ID3D11ShaderResourceView* srv = nullptr;
	if (FAILED(device->CreateShaderResourceView(texture, nullptr, &srv)));
}*/


ConstantBuffer Setup::CreateWorldMatrixConstantBuffer(Controller &controller)
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
		throw std::runtime_error("Failed to create World Matrix Constant Buffer");
	}

	return cbWorldMatrix;
}

ConstantBuffer Setup::CreatePixelShaderConstantBuffer(Controller &controller, Scene& scene)
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
		throw std::runtime_error("Failed to create Pixel Shader Constant Buffer");
	}

	return psConstBuffer;
}