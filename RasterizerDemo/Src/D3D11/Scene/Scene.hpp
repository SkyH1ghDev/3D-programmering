#pragma once

#include <vector>
#include "Camera.hpp"
#include "Mesh.hpp"

class Scene
{
public:
    Scene() = default;
    
    void Render();
    
private:
    Camera ActiveCamera;
    std::vector<Camera> CameraList;
    std::vector<Mesh> MeshList;
};