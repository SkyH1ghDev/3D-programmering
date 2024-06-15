#include "Sampler.hpp"

Sampler::Sampler(ID3D11SamplerState *sampler, std::optional<std::array<float, 4>> borderColour)
{
    this->_sampler = sampler;
}
