#include "GeometryShader.hpp"

GeometryShader::GeometryShader(ID3D11GeometryShader* geometryShader, ID3DBlob* shaderBlob)
{
    this->_geometryShader = geometryShader;
    this->_shaderBlob = shaderBlob;
}

GeometryShader::~GeometryShader()
{
    this->_geometryShader->Release();
    this->_shaderBlob->Release();
}

void GeometryShader::AddConstantBuffer(ConstantBuffer buffer)
{
    this->_constantBuffers.emplace_back(buffer);
}

std::vector<ConstantBuffer> GeometryShader::GetConstantBuffers()
{
    return this->_constantBuffers;
}

ID3DBlob* GeometryShader::GetShaderBlob() const
{
    return this->_shaderBlob;
}


ID3D11GeometryShader* GeometryShader::GetGeometryShader()
{
    return this->_geometryShader;
}




