#pragma once

#include "Shader.hpp"

class DomainShader : public Shader
{
public:

private:
    ID3D11DomainShader* _domainShader = nullptr;
};
