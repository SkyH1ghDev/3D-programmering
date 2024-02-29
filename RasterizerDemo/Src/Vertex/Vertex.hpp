#pragma once

#include <array>

class Vertex
{
public:

    Vertex() = default;
    
    Vertex(const std::array<float, 3> position, const std::array<float, 3> normal, const std::array<float, 2> uv)
    {
        for (int i = 0; i < 3; ++i)
        {
            this->_position[i] = position[i];
        }
        this->_position[3] = 1.0f;

        for (int i = 0; i < 3; ++i)
        {
            this->_normal[i] = normal[i];
        }
        this->_normal[3] = 0.0f;

        for (int i = 0; i < 2; ++i)
        {
            this->_uv[i] = uv[i];
        }
    }

    // Gets position of a Vertex
    std::array<float, 4> Position()
    {
        return this->_position;
    }

    // Sets position of a Vertex
    void SetPosition(const std::array<float, 3>& newPos)
    {
        for (int i = 0; i < 3; ++i)
        {
            this->_position[i] = newPos[i];
        }

        this->_position[3] = 1.0f;
    }

    // Gets normal of a Vertex
    std::array<float, 4> Normal()
    {
        return this->_normal;
    }

    // Sets normal of a Vertex
    void SetNormal(const std::array<float, 3>& newNormal)
    {
        for (int i = 0; i < 3; ++i)
        {
            this->_normal[i] = newNormal[i];
        }

        this->_normal[3] = 0.0f;
    }

    // Gets uv of a Vertex
    std::array<float, 2> UV()
    {
        return this->_uv;
    }

    // Sets uv of a Vertex
    void SetUV(const std::array<float, 2>& newUV)
    {
        for (int i = 0; i < 2; ++i)
        {
            this->_uv[i]= newUV[i];
        }
    }
    
private:
    std::array<float, 4> _position = { 0, 0, 0, 1 };
    std::array<float, 4> _normal= { 0, 0, 0, 0 };
    std::array<float, 2> _uv= { 0, 0 };
};
