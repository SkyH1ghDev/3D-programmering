#include "PixelShader.hpp"

PixelShader::PixelShader(ID3D11PixelShader* pixelShader, ID3DBlob* shaderBlob) : Shader()
{
    this->_pixelShader = pixelShader;
    this->_shaderBlob = shaderBlob;
}

PixelShader::~PixelShader()
{
    this->_shaderBlob->Release();
    this->_pixelShader->Release();
}

void PixelShader::AddConstantBuffer(ConstantBuffer buffer)
{
    this->_constantBuffers.emplace_back(buffer);
}


ID3DBlob *PixelShader::GetShaderBlob() const
{
    return this->_shaderBlob;
}

std::vector<ConstantBuffer> PixelShader::GetConstantBuffers()
{
    return this->_constantBuffers;
}


ID3D11PixelShader *PixelShader::GetPixelShader()
{
    return this->_pixelShader;
}

