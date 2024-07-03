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

    void BindShader(ID3D11DeviceContext *context) const override;
    ID3DBlob* GetShaderBlob() const override;
    
    ID3D11VertexShader* GetVertexShader();
    
private:
    ID3D11VertexShader* _vertexShader = nullptr;
};
