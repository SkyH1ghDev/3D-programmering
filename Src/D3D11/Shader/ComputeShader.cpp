﻿#include "ComputeShader.hpp"

ComputeShader::ComputeShader(ID3D11ComputeShader* computeShader, ID3DBlob* shaderBlob) : Shader()
{
    this->_computeShader = computeShader;
    this->_shaderBlob = shaderBlob;
}

ComputeShader::~ComputeShader()
{
    this->_computeShader->Release();
    this->_shaderBlob->Release();
}

void ComputeShader::AddConstantBuffer(ConstantBuffer buffer)
{
    this->_constantBuffers.emplace_back(buffer);
}

std::vector<ConstantBuffer> ComputeShader::GetConstantBuffers()
{
    return this->_constantBuffers;
}

ID3DBlob* ComputeShader::GetShaderBlob() const
{
    return this->_shaderBlob;
}

ID3D11ComputeShader* ComputeShader::GetComputeShader()
{
    return this->_computeShader;
}






