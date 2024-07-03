#pragma once

#include "Shader.hpp"

class PixelShader : public Shader
{
public:
    PixelShader() = delete;
    ~PixelShader() override;
    PixelShader(ID3D11PixelShader* vertexShader, ID3DBlob* shaderBlob);

    PixelShader(const PixelShader& other) = delete;
    PixelShader& operator=(const PixelShader& other) = delete;
    PixelShader(PixelShader&& other) = delete;
    PixelShader& operator=(PixelShader&& other) = delete;

    void BindShader(ID3D11DeviceContext *context) const override;
    ID3DBlob* GetShaderBlob() const override;
    
    ID3D11PixelShader* GetPixelShader();
    
private:
    ID3D11PixelShader* _pixelShader = nullptr;
};
