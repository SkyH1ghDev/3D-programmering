#pragma once

#include <d3d11.h>
#include <string>

#include "VertexBuffer.hpp"

class PipelineHelper
{
public:
	PipelineHelper() = default;
	
	bool SetupPipeline(ID3D11Device * device, VertexBuffer& vertexBuffer, ID3D11VertexShader *& vShader,
    				   ID3D11PixelShader *& pShader, ID3D11InputLayout *& inputLayout, ID3D11Texture2D*& texture,
    				   ID3D11ShaderResourceView*& textureSRV, ID3D11SamplerState*& textureSamplerState, unsigned char*& imageData);
	
private:
	
	bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, std::string& vShaderByteCode);
	bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const std::string& vShaderByteCode);
	VertexBuffer CreateVertexBuffer(HRESULT& hr, ID3D11Device* device, VertexBuffer& vertexBuffer);
	bool CreateTexture(ID3D11Device* device, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureSRV, unsigned char*& imageData);
	bool CreateSamplerState(ID3D11Device* device, ID3D11SamplerState*& samplerState);
};
