#pragma once

#include <vector>
#include "Camera.hpp"

class Scene
{
public:
    Camera ActiveCamera;
    std::vector<Camera> CameraList;
    
};
