#include "Shader.hpp"

Shader::Shader(Shader&& other) noexcept
{
    this->_shaderBlob = other._shaderBlob;
}

Shader::~Shader()
{
    this->_shaderBlob->Release();
}