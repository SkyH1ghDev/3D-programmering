#include "PipelineHelper.hpp"

#include <string>
#include <iostream>
#include <d3dcompiler.h>

bool PipelineHelper::LoadShaderBlob(ID3DBlob*& shaderBlob, ShaderType shaderType, LPCWSTR hlslPath)
{
	std::string target;
	
	switch (shaderType)
	{
		case ShaderType::VERTEX_SHADER:
			target = "vs_5_0";
			break;
		
		case ShaderType::HULL_SHADER:
			target = "hs_5_0";
			break;
		
		case ShaderType::DOMAIN_SHADER:
			target = "ds_5_0";
			break;
		
		case ShaderType::GEOMETRY_SHADER:
			target = "gs_5_0";
			break;
		
		case ShaderType::PIXEL_SHADER:
			target = "ps_5_0";
			break;
		
		case ShaderType::COMPUTE_SHADER:
			target = "cs_5_0";
			break;
	}

	if (target.empty())
	{
		std::cerr << "Could not assign a compilation target \n";
		return false;
	}

	ID3DBlob* errorBlob;
	if (FAILED(D3DCompileFromFile(hlslPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", target.c_str(), 0, 0, &shaderBlob, &errorBlob)))
	{
		std::cerr << "Compilation error " << static_cast<char*>(errorBlob->GetBufferPointer()) << "\n";
		errorBlob->Release();
		return false; 
	}
	
	return true;
}

bool PipelineHelper::LoadVertexShader(ID3D11Device *device, ID3D11VertexShader*& vertexShader, ID3DBlob *vertexShaderBlob)
{
	if (FAILED(device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader)))
	{
		std::cerr << "Failed to create vertex shader! \n";
		return false;
	}
	
	return true;
}

bool PipelineHelper::LoadHullShader(ID3D11Device* device, ID3D11HullShader*& hullShader, ID3DBlob* hullShaderBlob)
{
	if (FAILED(device->CreateHullShader(hullShaderBlob->GetBufferPointer(), hullShaderBlob->GetBufferSize(), nullptr, &hullShader)))
	{
		std::cerr << "Failed to create hull shader! \n";
		return false;
	}

	return true;
}

bool PipelineHelper::LoadDomainShader(ID3D11Device* device, ID3D11DomainShader*& domainShader, ID3DBlob* domainShaderBlob)
{
	
	if (FAILED(device->CreateDomainShader(domainShaderBlob->GetBufferPointer(), domainShaderBlob->GetBufferSize(), nullptr, &domainShader)))
	{
		std::cerr << "Failed to create hull shader! \n";
		return false;
	}

	return true;
}

bool PipelineHelper::LoadGeometryShader(ID3D11Device* device, ID3D11GeometryShader*& geometryShader, ID3DBlob* geometryShaderBlob)
{
	if (FAILED(device->CreateGeometryShader(geometryShaderBlob->GetBufferPointer(), geometryShaderBlob->GetBufferSize(), nullptr, &geometryShader)))
	{
		std::cerr << "Failed to create hull shader! \n";
		return false;
	}

	return true;
}


bool PipelineHelper::LoadPixelShader(ID3D11Device *device, ID3D11PixelShader*& pixelShader, ID3DBlob* pixelShaderBlob)
{
	if (FAILED(device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader)))
	{
		std::cerr << "Failed to create pixel shader!" << "\n";
		return false;
	}

	return true;
}

bool PipelineHelper::LoadComputeShader(ID3D11Device* device, ID3D11ComputeShader*& computeShader, ID3DBlob* computeShaderBlob)
{
	if (FAILED(device->CreateComputeShader(computeShaderBlob->GetBufferPointer(), computeShaderBlob->GetBufferSize(), nullptr, &computeShader)))
	{
		std::cerr << "Failed to create compute shader!" << "\n";
		return false;
	}

	return true;
}

bool PipelineHelper::CreateRasterizer(ID3D11Device* device, ID3D11RasterizerState*& rasterizerState)
{
	D3D11_RASTERIZER_DESC rasterizerDesc;

	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = TRUE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	
	HRESULT hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	return !FAILED(hr);
}


bool PipelineHelper::CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const void *vertexShaderByteCode, size_t vertexShaderByteCodeLength)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, 3, vertexShaderByteCode, vertexShaderByteCodeLength, &inputLayout);

	return !FAILED(hr);
}

bool PipelineHelper::CreateSamplerState(ID3D11Device* device, ID3D11SamplerState*& samplerState)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	return !(FAILED(hr));
}