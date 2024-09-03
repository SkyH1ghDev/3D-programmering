#pragma once

#include <array>

class Vertex
{
public:
    
    Vertex() = default;
    Vertex(const std::array<float, 3>& position, const std::array<float, 3>& normal, const std::array<float, 2>& uv);
    Vertex(const std::array<float, 4>& position, const std::array<float, 4>& normal, const std::array<float, 2>& uv);

    // Gets position of a Vertex
    std::array<float, 4> Position() const;

    // Sets position of a Vertex
    void SetPosition(const std::array<float, 3>& newPos);
    void SetPosition(const std::array<float, 4>& newPos);

    // Gets normal of a Vertex
    std::array<float, 4> Normal();

    // Sets normal of a Vertex
    void SetNormal(const std::array<float, 3>& newNormal);
    void SetNormal(const std::array<float, 4>& newNormal);

    // Gets uv of a Vertex
    std::array<float, 2> UV();

    // Sets uv of a Vertex
    void SetUV(const std::array<float, 2>& newUV);

private:
    std::array<float, 4> _position = { 0, 0, 0, 1 };
    std::array<float, 4> _normal= { 0, 0, 0, 0 };
    std::array<float, 2> _uv= { 0, 0 };
    
};