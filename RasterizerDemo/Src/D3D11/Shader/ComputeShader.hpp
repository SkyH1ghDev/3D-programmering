#pragma once

#include "Shader.hpp"

class ComputeShader : public Shader
{
public:

private:
    ID3D11ComputeShader* _computeShader = nullptr;
};
