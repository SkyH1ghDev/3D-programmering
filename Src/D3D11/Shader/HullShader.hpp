#pragma once

#include "Shader.hpp"

class HullShader : public Shader
{
public:

    HullShader() = delete;
    ~HullShader() override;
    HullShader(ID3D11HullShader* hullShader, ID3DBlob* shaderBlob);

    HullShader(const HullShader& other) = delete;
    HullShader& operator=(const HullShader& other) = delete;
    HullShader(HullShader&& other) = delete;
    HullShader& operator=(HullShader&& other) = delete;

    void AddConstantBuffer(ConstantBuffer buffer) override;
    
    ID3DBlob* GetShaderBlob() const override;
    std::vector<ConstantBuffer> GetConstantBuffers() override;
    
    ID3D11HullShader* GetHullShader();

private:
    ID3D11HullShader* _hullShader = nullptr;
};
