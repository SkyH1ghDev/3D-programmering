#include "HullShader.hpp"

HullShader::HullShader(ID3D11HullShader* hullShader, ID3DBlob* shaderBlob)
{
    this->_hullShader = hullShader;
    this->_shaderBlob = shaderBlob;
}

HullShader::~HullShader()
{
    this->_hullShader->Release();
    this->_shaderBlob->Release();
}

void HullShader::AddConstantBuffer(ConstantBuffer buffer)
{
    this->_constantBuffers.emplace_back(buffer);
}

std::vector<ConstantBuffer> HullShader::GetConstantBuffers()
{
    return this->_constantBuffers;
}

ID3DBlob* HullShader::GetShaderBlob() const
{
    return this->_shaderBlob;
}

ID3D11HullShader* HullShader::GetHullShader()
{
    return this->_hullShader;
}





