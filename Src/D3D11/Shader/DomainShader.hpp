#pragma once

#include "Shader.hpp"

class DomainShader : public Shader
{
public:

    DomainShader() = delete;
    ~DomainShader() override;
    DomainShader(ID3D11DomainShader* domainShader, ID3DBlob* shaderBlob);

    DomainShader(const DomainShader& other) = delete;
    DomainShader& operator=(const DomainShader& other) = delete;
    DomainShader(DomainShader&& other) = delete;
    DomainShader& operator=(DomainShader&& other) = delete;

public:
    void AddConstantBuffer(ConstantBuffer buffer) override;
    
    ID3DBlob* GetShaderBlob() const override;
    std::vector<ConstantBuffer> GetConstantBuffers() override;
    
    ID3D11DomainShader* GetDomainShader();

private:
    ID3D11DomainShader* _domainShader = nullptr;
};
