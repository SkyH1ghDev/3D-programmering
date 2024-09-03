#pragma once

#include "Shader.hpp"

class HullShader : public Shader
{
public:

private:
    ID3D11HullShader* _hullShader = nullptr;
};
