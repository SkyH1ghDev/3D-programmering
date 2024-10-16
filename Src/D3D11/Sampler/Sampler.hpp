#pragma once

#include <optional> // Needs C++ 17 or higher
#include <array>

#include <d3d11_4.h>

class Sampler
{
public:
	Sampler() = delete;
	Sampler(ID3D11SamplerState* sampler, std::optional<std::array<float, 4>> borderColour = std::nullopt);
	~Sampler();
	Sampler(const Sampler& other) = delete;
	Sampler& operator=(const Sampler& other) = delete;
	Sampler(Sampler&& other) = delete;
	Sampler& operator=(Sampler&& other) = delete;

	ID3D11SamplerState* GetSamplerState() const;
	
private:
	ID3D11SamplerState* _sampler = nullptr;
};