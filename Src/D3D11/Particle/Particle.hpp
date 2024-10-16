#pragma once
#include "StructuredBuffer.hpp"
#include "UnorderedAccessView.hpp"

class Particle
{
public:
    

    
private:
    StructuredBuffer _structuredBuffer;
    UnorderedAccessView _uav;
    ShaderResourceView _srv;
};
