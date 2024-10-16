﻿#include "Scene.hpp"

Scene::Scene(HRESULT& hr, ID3D11Device* device, ProjectionInfo& mainCamProjInfo, DX::XMFLOAT4& mainCamPosition)
{
    Camera mainCam(hr, device, mainCamProjInfo, mainCamPosition);
    
    this->_cameraList.push_back(std::move(mainCam));
}

Scene::Scene(Camera& camera)
{
    this->_cameraList.push_back(std::move(camera));
}

Scene::Scene(std::vector<Camera> cameraList)
{
    this->_cameraList = cameraList;
}

void Scene::AddMesh(Mesh& mesh)
{

    std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>(mesh);
    
    this->_meshList.push_back(meshPtr);

    if (mesh.IsOscillating())
    {
        this->_oscillatingMeshList.push_back(meshPtr);
    }

    if (mesh.IsOscillating() == false)
    {
        this->_quadTree.AddElement(meshPtr, meshPtr->GetBoundingBox());
    }
}

Camera& Scene::GetCurrentCamera()
{
    return this->_cameraList.at(this->_currentCameraIndex);
}

Camera& Scene::GetMainCamera()
{
    return this->_cameraList.at(0);
}


void Scene::IncrementCameraIndex()
{
    ++this->_currentCameraIndex;
    this->_currentCameraIndex %= this->_cameraList.size();
}

Mesh& Scene::GetMeshAt(size_t index) const
{
    return *this->_meshList.at(index);
}

std::shared_ptr<Mesh> Scene::GetMeshPtrAt(size_t index) const
{
    return this->_meshList.at(index);
}


size_t Scene::GetNumMeshes() const
{
    return this->_meshList.size();
}

Mesh& Scene::GetOscillatingMeshAt(size_t idx) const
{
    return *this->_oscillatingMeshList.at(idx);
}

std::shared_ptr<Mesh> Scene::GetOscillatingMeshPtrAt(size_t index) const
{
    return this->_oscillatingMeshList.at(index);
}


size_t Scene::GetNumOscillatingMeshes() const
{
    return this->_oscillatingMeshList.size();
}

const QuadTree<Mesh>& Scene::GetQuadTree() const
{
    return this->_quadTree;
}



