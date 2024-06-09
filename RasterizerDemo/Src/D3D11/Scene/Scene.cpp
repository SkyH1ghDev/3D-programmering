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
    for (Camera& camera : cameraList)
    {
        this->_cameraList.push_back(std::move(camera));
    }
}

void Scene::Render()
{
}

void Scene::SetMeshes(std::vector<Mesh> meshList)
{
    for (Mesh& mesh : meshList)
    {
        this->_meshList.push_back(std::move(mesh));
    }
}

Camera& Scene::GetCurrentCamera()
{
    return this->_cameraList.at(this->_currentCameraIndex);
}