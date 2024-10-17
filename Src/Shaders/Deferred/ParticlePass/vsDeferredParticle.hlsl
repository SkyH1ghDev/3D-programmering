struct Particle
{
    float4 ParticlePosition;
    float4 ParticleColour;
};

StructuredBuffer<Particle> Particles : register(t0);

float4 main(uint vertexID : SV_VertexID) : SV_POSITION
{
    return Particles[vertexID].ParticlePosition;
}