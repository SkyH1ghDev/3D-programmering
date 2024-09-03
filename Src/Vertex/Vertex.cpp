#include "Vertex.hpp"

Vertex::Vertex(const std::array<float, 3>& position, const std::array<float, 3>& normal, const std::array<float, 2>& uv)
{
    for (int i = 0; i < 3; ++i)
    {
        this->_position[i] = position[i];
        this->_normal[i] = normal[i];
    }
    
    this->_position[3] = 1.0f;
    this->_normal[3] = 0.0f;

    for (int i = 0; i < 2; ++i)
    {
        this->_uv[i] = uv[i];
    }
}

Vertex::Vertex(const std::array<float, 4>& position, const std::array<float, 4>& normal, const std::array<float, 2>& uv)
{
    for (int i = 0; i < 4; ++i)
    {
        this->_position[i] = position[i];
        this->_normal[i] = normal[i];
    }

    for (int i = 0; i < 2; ++i)
    {
        this->_uv[i] = uv[i];
    }
}

std::array<float, 4> Vertex::Position() const
{
    return this->_position;
}

void Vertex::SetPosition(const std::array<float, 3>& newPos)
{
    for (int i = 0; i < 3; ++i)
    {
        this->_position[i] = newPos[i];
    }

    this->_position[3] = 1.0f;
}

void Vertex::SetPosition(const std::array<float, 4>& newPos)
{
    for(int i = 0; i < 4; ++i)
    {
        this->_position[i] = newPos[i];
    }
}

std::array<float, 4> Vertex::Normal()
{
    return this->_normal;
}

void Vertex::SetNormal(const std::array<float, 3>& newNormal)
{
    for (int i = 0; i < 3; ++i)
    {
        this->_normal[i] = newNormal[i];
    }

    this->_normal[3] = 0.0f;
}

void Vertex::SetNormal(const std::array<float, 4>& newNormal)
{
    for (int i = 0; i < 4; ++i)
    {
        this->_normal[i] = newNormal[i];
    }
}

std::array<float, 2> Vertex::UV()
{
    return this->_uv;
}

void Vertex::SetUV(const std::array<float, 2>& newUV)
{
    for (int i = 0; i < 2; ++i)
    {
        this->_uv[i]= newUV[i];
    }
}
