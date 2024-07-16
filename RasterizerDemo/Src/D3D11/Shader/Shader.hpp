#pragma once

#include <d3d11_4.h>
#include <vector>

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
	virtual std::vector<ID3D11Buffer*> GetConstantBuffers() = 0;
	
	virtual void AddConstantBuffer(ID3D11Buffer* buffer) = 0;
	// UNUSED
	// virtual void BindShader(ID3D11DeviceContext* context) const = 0;
	
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
	std::vector<ID3D11Buffer*> _constantBuffers;

	/*union
	{
		ID3D11VertexShader* vertex = nullptr;
		ID3D11HullShader* hull;
		ID3D11DomainShader* domain;
		ID3D11GeometryShader* geometry;
		ID3D11PixelShader* pixel;
		ID3D11ComputeShader* compute;
	} _shader;*/

};