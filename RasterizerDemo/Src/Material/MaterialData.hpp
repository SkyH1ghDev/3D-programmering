#pragma once

#include <string>
#include <vector>

class MaterialData
{
public:
    std::string materialName;
    std::vector<unsigned char> diffuseMap;
    std::vector<unsigned char> specularMap;
    std::vector<unsigned char> ambientMap;
    int specularExponent;
};

