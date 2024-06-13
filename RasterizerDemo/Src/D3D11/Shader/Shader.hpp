#pragma once

#include <d3d11_4.h>

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
	Shader() = delete;
	~Shader();
	Shader(ShaderType type, ID3D11VertexShader* vertexShader, ID3DBlob* shaderBlob);
	Shader(ShaderType type, ID3D11HullShader* hullShader, ID3DBlob* shaderBlob);
	Shader(ShaderType type, ID3D11DomainShader* domainShader, ID3DBlob* shaderBlob);
	Shader(ShaderType type, ID3D11GeometryShader* geometryShader, ID3DBlob* shaderBlob);
	Shader(ShaderType type, ID3D11PixelShader* pixelSHader, ID3DBlob* shaderBlob);
	Shader(ShaderType type, ID3D11ComputeShader* computeShader, ID3DBlob* shaderBlob);
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader(Shader&& other) = delete;
	Shader& operator=(Shader&& other) = delete;

	void Initialize(ID3D11Device* device, ShaderType shaderType, const void* dataPtr, size_t dataSize);
	void Initialize(ID3D11Device* device, ShaderType shaderType, const char* csoPath);

	const void* GetShaderByteData() const;
	size_t GetShaderByteSize() const;

	void BindShader(ID3D11DeviceContext* context) const;
	
private:

	ShaderType _type;

	union
	{
		ID3D11VertexShader* vertex = nullptr;
		ID3D11HullShader* hull;
		ID3D11DomainShader* domain;
		ID3D11GeometryShader* geometry;
		ID3D11PixelShader* pixel;
		ID3D11ComputeShader* compute;
	} _shader;

	ID3DBlob* _shaderBlob = nullptr;
};