#include "SetupHelper.hpp"

#include <iostream>
#include <d3dcompiler.h>
#include "WindowHelper.hpp"
#include "D3D11Helper.hpp"
#include "Configuration.hpp"
#include "FileReader.hpp"
#include "PipelineHelper.hpp"

HWND SetupHelper::SetupWindow(HINSTANCE hInstance, int nCmdShow) 
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

D3D11Controller SetupHelper::SetupController(HWND window)
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

RenderTarget SetupHelper::SetupRenderTarget(D3D11Controller &controller)
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

Scene SetupHelper::SetupScene(D3D11Controller &controller)
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
		}
		
		scene.AddMesh(mesh);
	}

	return scene;
}
Shader SetupHelper::SetupShader(D3D11Controller& controller, ShaderType shaderType, LPCWSTR csoPath)
{
	PipelineHelper pipelineHelper;
	ID3DBlob* shaderBlob;

	if (!pipelineHelper.LoadShaderBlob(shaderBlob, csoPath))
	{
		std::cerr << "Failed to Read Shader Data \n Exiting... \n";
		shaderBlob->Release();
		exit(-1);
	}
	
	switch (shaderType)
	{
		// TODO: Pair up input layout with vertex shader somehow
		
		case ShaderType::VERTEX_SHADER:
			if (!pipelineHelper.LoadVertexShader(controller.GetDevice(), vertexShader, shaderBlob))
			{
				std::cerr << "Could not Compile Vertex Shader \n Exiting... \n";
				shaderBlob->Release();
				exit(-1);
			}
		
			ID3D11VertexShader* vertexShader;
			std::string vertexShaderByteCode;
			pipelineHelper.LoadVertexShader(controller.GetDevice(), vertexShader, vertexShaderByteCode);
			break;
		
		case ShaderType::HULL_SHADER:
			break;
		
		case ShaderType::DOMAIN_SHADER:
			break;

		case ShaderType::GEOMETRY_SHADER:
			break;

		case ShaderType::PIXEL_SHADER:
			break;

		case ShaderType::COMPUTE_SHADER:
			break;
		
		default:
			std::cerr << "Incorrect Shader Type (How did we get here?) \n Exiting... \n";
			exit(-1);
	}
	
}

