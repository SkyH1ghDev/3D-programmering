#pragma once

#include <array>

class Vertex
{
public:
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
    
    std::array<float, 4> Position()
    {
        return _position;
    }

    std::array<float, 4> Normal()
    {
        return _normal;
    }

    std::array<float, 2> UV()
    {
        return _uv;
    }
    
private:
    std::array<float, 4> _position = { 0, 0, 0, 1 };
    std::array<float, 4> _normal= { 0, 0, 0, 0 };
    std::array<float, 2> _uv= { 0, 0 };
};
