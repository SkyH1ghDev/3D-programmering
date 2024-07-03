#include "Shader.hpp"

/*Shader::Shader(ShaderType type, ID3D11VertexShader *vertexShader, ID3DBlob *shaderBlob)
{
    this->_shader.vertex = vertexShader;
    this->_type = type;
    this->_shaderBlob = shaderBlob;
}


Shader::Shader(ShaderType type, ID3D11PixelShader *pixelShader, ID3DBlob *shaderBlob)
{
    this->_shader.pixel = pixelShader;
    this->_type = type;
    this->_shaderBlob = shaderBlob;
}*/

Shader::Shader(Shader&& other) noexcept
{
    this->_shaderBlob = other._shaderBlob;
}

Shader::~Shader()
{
    this->_shaderBlob->Release();
    
    /*switch (this->_type)
    {
        case ShaderType::VERTEX_SHADER:
            this->_shader.vertex->Release();
            break;
        
        case ShaderType::HULL_SHADER:
            this->_shader.hull->Release();
            break;
        
        case ShaderType::DOMAIN_SHADER:
            this->_shader.domain->Release();
            break;
        
        case ShaderType::GEOMETRY_SHADER:
            this->_shader.geometry->Release();
            break;
        
        case ShaderType::PIXEL_SHADER:
            this->_shader.pixel->Release();
            break;
        
        case ShaderType::COMPUTE_SHADER:
            this->_shader.compute->Release();
            break;
    }*/
}

/*ID3DBlob *Shader::GetShaderBlob() const
{
    return this->_shaderBlob;
}*/



