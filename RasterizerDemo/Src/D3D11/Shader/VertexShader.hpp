#pragma once

#include "Shader.hpp"

class VertexShader : public Shader
{
public:
    VertexShader() = delete;
    ~VertexShader() override;
    VertexShader(ID3D11VertexShader* vertexShader, ID3DBlob* shaderBlob);

    VertexShader(const VertexShader& other) = delete;
    VertexShader& operator=(const VertexShader& other) = delete;
    VertexShader(VertexShader&& other) = delete;
    VertexShader& operator=(VertexShader&& other) = delete;

    void AddConstantBuffer(ID3D11Buffer* buffer) override;
    
    ID3DBlob* GetShaderBlob() const override;
    std::vector<ID3D11Buffer*> GetConstantBuffers() override;
    
    ID3D11VertexShader* GetVertexShader();
    
private:
    ID3D11VertexShader* _vertexShader = nullptr;
};
