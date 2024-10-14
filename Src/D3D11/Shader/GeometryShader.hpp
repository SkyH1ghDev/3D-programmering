#pragma once

#include "Shader.hpp"

class GeometryShader : public Shader
{
public:

    GeometryShader() = delete;
    ~GeometryShader() override;
    GeometryShader(ID3D11GeometryShader* geometryShader, ID3DBlob* shaderBlob);

    GeometryShader(const GeometryShader& other) = delete;
    GeometryShader& operator=(const GeometryShader& other) = delete;
    GeometryShader(GeometryShader&& other) = delete;
    GeometryShader& operator=(GeometryShader&& other) = delete;

    void AddConstantBuffer(ConstantBuffer buffer) override;
    
    ID3DBlob* GetShaderBlob() const override;
    std::vector<ConstantBuffer> GetConstantBuffers() override;
    
    ID3D11GeometryShader* GetGeometryShader();

private:
    ID3D11GeometryShader* _geometryShader = nullptr;
};
