#include "Scene.hpp"

Scene::Scene(HRESULT& hr, ID3D11Device* device)
{
    Camera mainCam(hr, device);
    
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

void Scene::Render()
{
}

void Scene::CreateMeshes(std::vector<Mesh> meshList)
{
    this->_meshList = meshList;
}

void Scene::AddMesh(Mesh& mesh)
{
    this->_meshList.emplace_back(std::move(mesh));
}


Camera& Scene::GetCurrentCamera()
{
    return this->_cameraList.at(this->_currentCameraIndex);
}