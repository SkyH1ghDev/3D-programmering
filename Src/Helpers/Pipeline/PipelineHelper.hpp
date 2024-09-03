#pragma once

#include <d3d11.h>
#include "Shader.hpp"

class PipelineHelper
{
public:
	PipelineHelper() = default;

	bool LoadShaderBlob(ID3DBlob*& shaderBlob, ShaderType shaderType, LPCWSTR hlslPath);
	bool LoadVertexShader(ID3D11Device* device, ID3D11VertexShader*& vertexShader, ID3DBlob* vertexShaderBlob);
	bool LoadHullShader(ID3D11Device* device, ID3D11HullShader*& hullShader, ID3DBlob* hullShaderBlob);
	bool LoadDomainShader(ID3D11Device* device, ID3D11DomainShader*& domainShader, ID3DBlob* domainShaderBlob);
	bool LoadGeometryShader(ID3D11Device* device, ID3D11GeometryShader*& geometryShader, ID3DBlob* geometryShaderBlob);
	bool LoadPixelShader(ID3D11Device* device, ID3D11PixelShader*& pixelShader, ID3DBlob* pixelShaderBlob);
	bool LoadComputeShader(ID3D11Device* device, ID3D11ComputeShader*& computeShader, ID3DBlob* computeShaderBlob);
	
	bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const void *vertexShaderByteCode, size_t vertexShaderByteCodeLength);
	bool CreateSamplerState(ID3D11Device* device, ID3D11SamplerState*& samplerState);
};