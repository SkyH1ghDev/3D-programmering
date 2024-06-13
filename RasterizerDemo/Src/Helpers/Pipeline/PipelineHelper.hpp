#pragma once

#include <d3d11.h>
#include <string>
#include "Shader.hpp"

class PipelineHelper
{
public:
	PipelineHelper() = default;
	
	bool SetupPipeline(ID3D11Device * device, ID3D11Buffer *& vertexBuffer, ID3D11VertexShader *& vShader,
    				   ID3D11PixelShader *& pShader, ID3D11InputLayout *& inputLayout, ID3D11Texture2D*& texture,
    				   ID3D11ShaderResourceView*& textureSRV, ID3D11SamplerState*& textureSamplerState, unsigned char*& imageData);

	bool LoadShaderBlob(ID3DBlob*& shaderBlob, ShaderType shaderType, LPCWSTR csoPath);
	bool LoadVertexShader(ID3D11Device* device, ID3D11VertexShader*& vertexShader, ID3DBlob* vertexShaderBlob);
	bool LoadHullShader(ID3D11Device* device, ID3D11HullShader*& hullShader, ID3DBlob* hullShaderBlob);
	bool LoadDomainShader(ID3D11Device* device, ID3D11DomainShader*& domainShader, ID3DBlob* domainShaderBlob);
	bool LoadGeometryShader(ID3D11Device* device, ID3D11GeometryShader*& geometryShader, ID3DBlob* geometryShaderBlob);
	bool LoadPixelShader(ID3D11Device* device, ID3D11PixelShader*& pixelShader, ID3DBlob* pixelShaderBlob);
	bool LoadComputeShader(ID3D11Device* device, ID3D11ComputeShader*& computeShader, ID3DBlob* computeShaderBlob);
	
private:
	
	//bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, std::string& vShaderByteCode);
	bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const std::string& vShaderByteCode);
	bool CreateVertexBuffer(ID3D11Device* device, ID3D11Buffer*& vertexBuffer);
	bool CreateTexture(ID3D11Device* device, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureSRV, unsigned char*& imageData);
	bool CreateSamplerState(ID3D11Device* device, ID3D11SamplerState*& samplerState);
};