cbuffer csBuffer : register(b0)
{
    float4 randomizedVector;
}

struct Particle
{
    float4 ParticlePosition;
    float4 ParticleColour;
};

RWStructuredBuffer<Particle> Particles : register (u0);

[numthreads(1, 1, 1)]
void main (uint3 DTid : SV_DispatchThreadID )
{
    Particle gettingProcessed = Particles[DTid.x];

    // Will make the particle vibrate
    gettingProcessed.ParticlePosition += randomizedVector;
    
    Particles[DTid.x] = gettingProcessed;
}