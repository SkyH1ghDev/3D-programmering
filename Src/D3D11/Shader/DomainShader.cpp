#include "DomainShader.hpp"

DomainShader::DomainShader(ID3D11DomainShader* domainShader, ID3DBlob* shaderBlob)
{
    this->_domainShader = domainShader;
    this->_shaderBlob = shaderBlob;
}

DomainShader::~DomainShader()
{
    this->_domainShader->Release();
    this->_shaderBlob->Release();
}

void DomainShader::AddConstantBuffer(ConstantBuffer buffer)
{
    this->_constantBuffers.emplace_back(buffer);
}

std::vector<ConstantBuffer> DomainShader::GetConstantBuffers()
{
    return this->_constantBuffers;
}

ID3DBlob* DomainShader::GetShaderBlob() const
{
    return this->_shaderBlob;
}

ID3D11DomainShader* DomainShader::GetDomainShader()
{
    return this->_domainShader;
}



