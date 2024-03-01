#pragma once

#include <vector>

class Face
{
public:
    std::vector<int> PositionIndices()
    {
        return this->_positionIndices;
    }

    void AddPositionIndex(const std::vector<int>& positionIndexVec)
    {
        for (auto index : positionIndexVec)
        {
            _positionIndices.emplace_back(index);
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
            _normalIndices.emplace_back(index);
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
            this->_uvIndices.emplace_back(index);
        }
    }

private:
    std::vector<int> _positionIndices;
    std::vector<int> _normalIndices;
    std::vector<int> _uvIndices;
};
