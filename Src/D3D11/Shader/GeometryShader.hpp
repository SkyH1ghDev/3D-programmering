#pragma once

#include "Shader.hpp"

class GeometryShader : public Shader
{
public:

private:
    ID3D11GeometryShader* _geometryShader = nullptr;
};
