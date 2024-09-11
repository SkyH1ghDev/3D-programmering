#pragma once

#include "Shader.hpp"

class ComputeShader : public Shader
{
public:

    ComputeShader() = delete;
    ~ComputeShader() override;
    ComputeShader(ID3D11ComputeShader* computeShader, ID3DBlob* shaderBlob);

    ComputeShader(const ComputeShader& other) = delete;
    ComputeShader& operator=(const ComputeShader& other) = delete;
    ComputeShader(ComputeShader&& other) = delete;
    ComputeShader& operator=(ComputeShader&& other) = delete;

    void AddConstantBuffer(ConstantBuffer buffer) override;
    
    ID3DBlob* GetShaderBlob() const override;
    std::vector<ConstantBuffer> GetConstantBuffers() override;
    
    ID3D11ComputeShader* GetComputeShader();
    
private:
    ID3D11ComputeShader* _computeShader = nullptr;
};
