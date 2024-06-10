﻿#include "Mesh.hpp"

#include <iostream>

Mesh::Mesh(HRESULT &hr, ID3D11Device *device, const MeshData &meshData)
{
    VertexInfo vertexInfo = meshData.VertexInfo;
    
    this->_vertexBuffer = VertexBuffer(hr, device, vertexInfo.SizeOfVertex, vertexInfo.NrOfVerticesInBuffer, vertexInfo.VertexVector); // Might work, unsure

	if (FAILED(hr))
	{
		std::cerr << "Creating Mesh VertexBuffer failed \n";
	}

    std::vector<SubMeshInfo> subMeshInfoVector = meshData.SubMeshInfoList;

    for (SubMeshInfo& subMeshInfo : subMeshInfoVector)
    {
		SubMesh subMesh(hr, device, subMeshInfo);
    	
    	this->_subMeshes.emplace_back(std::move(subMesh));
	}
}

void Mesh::BindMeshBuffers(ID3D11DeviceContext *context) const
{
	
}

void Mesh::PerformSubMeshDrawCall(ID3D11DeviceContext *context, size_t subMeshIndex) const
{
	
}

size_t Mesh::GetNrOfSubMeshes() const
{
	return this->_subMeshes.size();
}

ID3D11ShaderResourceView *Mesh::GetAmbientSRV(size_t subMeshIndex) const
{
	return this->_subMeshes.at(subMeshIndex).GetAmbientSRV();
}

ID3D11ShaderResourceView *Mesh::GetDiffuseSRV(size_t subMeshIndex) const
{
	return this->_subMeshes.at(subMeshIndex).GetDiffuseSRV();
}

ID3D11ShaderResourceView *Mesh::GetSpecularSRV(size_t subMeshIndex) const
{
	return this->_subMeshes.at(subMeshIndex).GetSpecularSRV();
}



