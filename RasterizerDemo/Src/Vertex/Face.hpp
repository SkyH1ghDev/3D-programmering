#pragma once

#include <vector>

class Face
{
public:
    std::vector<int> VertexPositionIndices()
    {
        return this->_positionIndices;
    }

    void AddVertexPositionIndex(const std::vector<int>& positionIndexVec)
    {
        for (auto index : positionIndexVec)
        {
            _positionIndices.push_back(index);
        }
    }

    std::vector<int> NormalIndices()
    {
        return this->_normalIndices;
    }

    void AddNormalIndex(const std::vector<int>& normalIndexVec)
    {
        for (auto index : normalIndexVec)
        {
            _normalIndices.push_back(index);
        }
    }

    std::vector<int> UVIndices()
    {
        return this->_uvIndices;
    }

    void AddUVIndex(const std::vector<int>& uvIndexVec)
    {
        for(auto index : uvIndexVec)
        {
            this->_uvIndices.push_back(index);
        }
    }

private:
    std::vector<int> _positionIndices;
    std::vector<int> _normalIndices;
    std::vector<int> _uvIndices;
};
