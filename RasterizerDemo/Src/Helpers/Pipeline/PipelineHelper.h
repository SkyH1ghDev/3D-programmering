#pragma once

#include <array>
#include <d3d11.h>

struct SimpleVertex
{
	std::array<float, 4> pos;
	std::array<float, 4> nrm;
	std::array<float, 2> uv;

	SimpleVertex(const SimpleVertex&) = default;
	SimpleVertex() = default;
	
	SimpleVertex(const std::array<float, 3>& position, const std::array<float, 3>& normal, const std::array<float, 2>& UV)
	{
		for (int i = 0; i < 2; ++i)
		{
			uv[i] = UV[i];
		}
		
		for (int i = 0; i < 3; ++i)
		{
			pos[i] = position[i];
			nrm[i] = normal[i];
		}

		pos[3] = 1.0f;
		nrm[3] = 0.0f;
	}

	SimpleVertex(const std::array<float, 4>& position, const std::array<float, 4>& normal, const std::array<float, 2>& UV)
	{
		for (int i = 0; i < 2; ++i)
		{
			uv[i] = UV[i];
		}
		
		for (int i = 0; i < 4; ++i)
		{
			pos[i] = position[i];
			nrm[i] = normal[i];
		}
	}
};

bool SetupPipeline(ID3D11Device * device, ID3D11Buffer *& vertexBuffer, ID3D11VertexShader *& vShader,
				   ID3D11PixelShader *& pShader, ID3D11InputLayout *& inputLayout, ID3D11Texture2D*& texture,
				   ID3D11ShaderResourceView*& textureSRV, ID3D11SamplerState*& textureSamplerState, unsigned char*& imageData);
