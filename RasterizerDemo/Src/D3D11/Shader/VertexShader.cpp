#include "VertexShader.hpp"

VertexShader::VertexShader(ID3D11VertexShader* vertexShader, ID3DBlob* shaderBlob) : Shader()
{
    this->_vertexShader = vertexShader;
    this->_shaderBlob = shaderBlob;
}

VertexShader::~VertexShader()
{
    this->_shaderBlob->Release();
    this->_vertexShader->Release();
}

void VertexShader::AddConstantBuffer(ID3D11Buffer* buffer)
{
    this->_constantBuffers.emplace_back(buffer);
}


ID3DBlob *VertexShader::GetShaderBlob() const
{
    return this->_shaderBlob;
}

std::vector<ID3D11Buffer*> VertexShader::GetConstantBuffers()
{
    return this->_constantBuffers;
}

ID3D11VertexShader *VertexShader::GetVertexShader()
{
    return this->_vertexShader;
}


