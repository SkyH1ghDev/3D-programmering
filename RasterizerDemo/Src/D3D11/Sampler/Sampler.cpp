#include "Sampler.hpp"

Sampler::Sampler(ID3D11SamplerState *sampler, std::optional<std::array<float, 4>> borderColour)
{
    this->_sampler = sampler;
}

Sampler::~Sampler()
{
    this->_sampler->Release();
}

ID3D11SamplerState *Sampler::GetSamplerState() const
{
    return this->_sampler;
}

