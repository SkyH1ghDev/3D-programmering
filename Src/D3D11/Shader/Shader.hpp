#pragma once

#include <d3d11_4.h>
#include <vector>

#include "ConstantBuffer.hpp"

enum class ShaderType
{
	VERTEX_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER
};

class Shader
{
public:
	
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) = delete;
	
	virtual ID3DBlob* GetShaderBlob() const = 0;
	virtual std::vector<ConstantBuffer> GetConstantBuffers() = 0;
	
	virtual void AddConstantBuffer(ConstantBuffer buffer) = 0;
	
protected:
	Shader() = default;
	virtual ~Shader();
	
	//Shader(ShaderType type, ID3D11VertexShader* vertexShader, ID3DBlob* shaderBlob);
	//Shader(ShaderType type, ID3D11HullShader* hullShader, ID3DBlob* shaderBlob);
	//Shader(ShaderType type, ID3D11DomainShader* domainShader, ID3DBlob* shaderBlob);
	//Shader(ShaderType type, ID3D11GeometryShader* geometryShader, ID3DBlob* shaderBlob);
	//Shader(ShaderType type, ID3D11PixelShader* pixelSHader, ID3DBlob* shaderBlob);
	//Shader(ShaderType type, ID3D11ComputeShader* computeShader, ID3DBlob* shaderBlob);
	


	//void Initialize(ID3D11Device* device, ShaderType shaderType, const void* dataPtr, size_t dataSize);
	//void Initialize(ID3D11Device* device, ShaderType shaderType, const char* csoPath);

protected:
	
	ID3DBlob* _shaderBlob = nullptr;
	std::vector<ConstantBuffer> _constantBuffers;
};