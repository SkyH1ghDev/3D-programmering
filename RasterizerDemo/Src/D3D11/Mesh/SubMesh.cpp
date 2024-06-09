#include "SubMesh.hpp"

SubMesh::SubMesh(size_t startIndexValue, size_t nrOfIndicesInSubMesh, float specularExponent,
    ID3D11ShaderResourceView* ambientTextureSRV, ID3D11Texture2D* ambientTexture2D,
    ID3D11ShaderResourceView* diffuseTextureSRV, ID3D11Texture2D* diffuseTexture2D,
    ID3D11ShaderResourceView* specularTextureSRV, ID3D11Texture2D* specularTexture2D)
{
    this->_startIndex = startIndexValue;
    this->_nrOfIndices = nrOfIndicesInSubMesh;
    this->_specularExponent = specularExponent;
    
    this->_ambientTextureSRV = ambientTextureSRV;
    this->_ambientTexture2D = ambientTexture2D;
    
    this->_diffuseTextureSRV = diffuseTextureSRV;
    this->_diffuseTexture2D = diffuseTexture2D;
    
    this->_specularTextureSRV = specularTextureSRV;
    this->_specularTexture2D = specularTexture2D;
}

SubMesh::~SubMesh()
{
    this->_ambientTextureSRV->Release();
    this->_ambientTexture2D->Release();
    
    this->_diffuseTextureSRV->Release();
    this->_diffuseTexture2D->Release();
    
    this->_specularTextureSRV->Release();
    this->_specularTexture2D->Release();
}

