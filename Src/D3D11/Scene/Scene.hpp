﻿#pragma once

#include <vector>
#include <memory>
#include "Camera.hpp"
#include "Mesh.hpp"

class Scene
{
public:
    Scene() = delete;
    ~Scene() = default;
    Scene(HRESULT& hr, ID3D11Device* device);
    Scene(Camera& camera);
    Scene(std::vector<Camera> cameraList);
    
    void Render();
    //void CreateMeshes(std::vector<Mesh> meshList);
    void AddMesh(Mesh& mesh);

    Camera& GetCurrentCamera();
    Mesh& GetMeshAt(size_t index) const;
    size_t GetNumMeshes() const;
    Mesh& GetOscillatingMeshAt(size_t idx) const;
    size_t GetNumOscillatingMeshes() const;
    
private:
    
    uint16_t _currentCameraIndex = 0; // Max 65535 Cameras
    std::vector<Camera> _cameraList;
    std::vector<std::shared_ptr<Mesh>> _meshList;
    std::vector<std::shared_ptr<Mesh>> _oscillatingMeshList;
};