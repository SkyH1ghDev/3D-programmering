#include "Mesh.hpp"

#include <iostream>

#include "Math.hpp"

Mesh::Mesh(HRESULT &hr, ID3D11Device *device, const MeshData &meshData)
{
    VertexInfo vertexInfo = meshData.VertexInfo;
	
    this->_vertexBuffer = VertexBuffer(hr, device, vertexInfo.SizeOfVertex, vertexInfo.NrOfVerticesInBuffer, vertexInfo.VertexVector);
	
	if (FAILED(hr))
	{
		std::cerr << "Creating Mesh VertexBuffer failed \n";
	}
	
    std::vector<SubMeshInfo> subMeshInfoVector = meshData.SubMeshInfoList;
	this->_currentPosition = meshData.MeshPosition;
	this->_boundingBox = DX::BoundingBox({this->_currentPosition.x, this->_currentPosition.y, this->_currentPosition.z}, {1.0f, 1.0f, 1.0f});
	

	this->_meshFlags = meshData.MeshFlags;

	this->_oscillationEndPosition = meshData.OscillationStartPosition;
	this->_oscillationStartPosition = this->_currentPosition;
	this->_oscillationTime = meshData.OscillationTime;
	this->_oscillationPeriod = meshData.OscillationPeriod;
	
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

void Mesh::PerformOscillation(float deltaTime)
{
	this->_currentPosition = Math::OscillateBetweenPoints(this->_oscillationTime, this->_oscillationStartPosition, this->_oscillationEndPosition);
	if (this->_oscillationTime > this->_oscillationPeriod)
	{
		this->_oscillationTime -= this->_oscillationPeriod;
	}
	this->_oscillationTime += 0.1f * deltaTime;
}

bool Mesh::IsOscillating() const
{
	return this->_meshFlags[0];
}


SubMesh& Mesh::GetSubMeshAt(size_t index)
{
	return this->_subMeshes.at(index);
}

size_t Mesh::GetNrOfSubMeshes() const
{
	return this->_subMeshes.size();
}

VertexBuffer Mesh::GetVertexBuffer()
{
	return this->_vertexBuffer;
}

DX::XMFLOAT4 Mesh::GetCurrentPosition() const
{
	return this->_currentPosition;
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

ID3D11ShaderResourceView *Mesh::GetTextureSRV(size_t subMeshIndex) const
{
	return this->_subMeshes.at(subMeshIndex).GetColourTextureSRV();
}

DX::BoundingBox& Mesh::GetBoundingBox()
{
	return this->_boundingBox;
}





