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

void VertexShader::BindShader(ID3D11DeviceContext *context) const
{
    
}

ID3DBlob *VertexShader::GetShaderBlob() const
{
    return this->_shaderBlob;
}


ID3D11VertexShader *VertexShader::GetVertexShader()
{
    return this->_vertexShader;
}


