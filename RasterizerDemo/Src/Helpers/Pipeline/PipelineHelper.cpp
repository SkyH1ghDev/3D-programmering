#include "PipelineHelper.hpp"
#include "VertexManager.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <d3dcompiler.h>

#include "stb_image.h"

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
	
	/*std::string shaderData;
	std::ifstream reader;
    
	reader.open("Build/VertexShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open VS file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vertexShader)))
	{
		std::cerr << "Failed to create vertex shader!" << std::endl;
		return false;
	}

	vertexShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	
	return true;*/
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


bool PipelineHelper::LoadPixelShader(ID3D11Device *device, ID3D11PixelShader*& pixelShader, ID3DBlob* pixelShaderBlob)
{
	if (FAILED(device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader)))
	{
		std::cerr << "Failed to create pixel shader!" << "\n";
		return false;
	}

	return true;
}


/*
bool PipelineHelper::LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, std::string& vShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open("Build/VertexShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open VS file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShader)))
	{
		std::cerr << "Failed to create vertex shader!" << std::endl;
		return false;
	}

	vShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	reader.open("Build/PixelShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open PS file!" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pShader)))
	{
		std::cerr << "Failed to create pixel shader!" << "\n";
		return false;
	}

	return true;
}
*/

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

bool PipelineHelper::CreateVertexBuffer(ID3D11Device* device, ID3D11Buffer*& vertexBuffer)
{
	std::vector<Vertex> teapot;

	VertexManager* vertexManagerInstance = VertexManager::GetInstance();

	for (const auto& face : vertexManagerInstance->FaceList)
	{
		for (int i = 0; i < face->VertexPositionIndices().size(); ++i) // PositionIndices, UVIndices and NormalIndices are all equal size, however only PositionIndices is guaranteed to have elements
		{

			Vertex vertex;

			vertex.SetPosition(vertexManagerInstance->PositionList.at(face->VertexPositionIndices().at(i)));
			vertex.SetNormal(vertexManagerInstance->NormalList.at(face->NormalIndices().at(i)));
			vertex.SetUV(vertexManagerInstance->UVList.at(face->UVIndices().at(i)));
			
			teapot.push_back(vertex);
		}
	}

	Vertex* teapotArr = new Vertex[teapot.size()];

	std::copy(teapot.begin(), teapot.end(), teapotArr);
	
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(*teapotArr) * vertexManagerInstance->numVerticesToDraw;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = teapotArr;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	delete[] teapotArr;
	return !FAILED(hr);
}

bool PipelineHelper::CreateTexture(ID3D11Device* device, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureSRV, unsigned char*& imageData)
{
	int textureHeight, textureWidth, rgbaChannels;
	
	imageData = stbi_load("Resources/Images/Pic.jpg", &textureWidth, &textureHeight, &rgbaChannels, 0);

	rgbaChannels = 4;
	
	std::vector<unsigned char> textureData;
	textureData.resize(textureHeight * textureWidth * rgbaChannels);

	int imageDataIndex = 0;
	for (int h = 0; h < textureHeight; ++h)
	{
		for (int w = 0; w < textureWidth; ++w)
		{
			unsigned int pos0 = (w + textureWidth * h) * rgbaChannels;

			textureData[pos0 + 0] = imageData[imageDataIndex++];
			textureData[pos0 + 1] = imageData[imageDataIndex++];
			textureData[pos0 + 2] = imageData[imageDataIndex++];
			textureData[pos0 + 3] = 255;
			
		}
	}
	
	D3D11_TEXTURE2D_DESC texture2DDesc;
	texture2DDesc.Width = textureWidth;
	texture2DDesc.Height = textureHeight;
	texture2DDesc.MipLevels = 1;
	texture2DDesc.ArraySize = 1;
	texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2DDesc.SampleDesc = DXGI_SAMPLE_DESC{1, 0};
	texture2DDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture2DDesc.CPUAccessFlags = 0;
	texture2DDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA texture2DData;
	texture2DData.pSysMem = &textureData[0];
	texture2DData.SysMemPitch = textureWidth * rgbaChannels;
	texture2DData.SysMemSlicePitch = 0;
	
	if (FAILED(device->CreateTexture2D(&texture2DDesc, &texture2DData, &texture)))
	{
		std::cerr << "Failed to Create Texture2D" << std::endl;
		return false;
	}
	
	HRESULT hr = device->CreateShaderResourceView(texture, nullptr, &textureSRV);
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

bool PipelineHelper::SetupPipeline(ID3D11Device* device, ID3D11Buffer*& vertexBuffer, ID3D11VertexShader*& vShader,
				   ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout, ID3D11Texture2D*& texture,
				   ID3D11ShaderResourceView*& textureSRV, ID3D11SamplerState*& samplerState, unsigned char*& imageData)
{
	//std::string vShaderByteCode;
	/*if (!LoadShaders(device, vShader, pShader, vShaderByteCode))
	{
		std::cerr << "Error loading shaders!" << std::endl;
		return false;
	}*/

	/*if (!CreateInputLayout(device, inputLayout, vShaderByteCode))
	{
		std::cerr << "Error creating input layout!" << std::endl;
		return false;
	}*/
	
	/*if (!(CreateVertexBuffer(device, vertexBuffer)))
	{
		std::cerr << "Error creating vertex buffer!" << std::endl;
		return false;
	}*/
	
	if (!(CreateTexture(device, texture, textureSRV, imageData)))
	{
		std::cerr << "Error creating texture!" << std::endl;
		return false;
	}

	if (!(CreateSamplerState(device, samplerState)))
	{
     		std::cerr << "Error creating sampler state!" << std::endl;
     		return false;
	}
	
	return true;
}