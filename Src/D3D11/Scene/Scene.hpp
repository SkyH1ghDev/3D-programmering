#pragma once

#include <vector>
#include <memory>
#include "Camera.hpp"
#include "Mesh.hpp"
#include "QuadTree.hpp"
#include "StructuredBuffer.hpp"

class Scene
{
public:
    Scene() = delete;
    ~Scene() = default;
    Scene(HRESULT& hr, ID3D11Device* device, ProjectionInfo& mainCamProjInfo, DX::XMFLOAT4& mainCamPosition);
    Scene(Camera& camera);
    Scene(std::vector<Camera> cameraList);
    
    void Render();
    //void CreateMeshes(std::vector<Mesh> meshList);
    void AddMesh(Mesh& mesh);

    Camera& GetCurrentCamera();
    Camera& GetMainCamera();
    void IncrementCameraIndex();
    
    Mesh& GetMeshAt(size_t index) const;
    std::shared_ptr<Mesh> GetMeshPtrAt(size_t index) const;
    size_t GetNumMeshes() const;
    Mesh& GetOscillatingMeshAt(size_t idx) const;
    std::shared_ptr<Mesh> GetOscillatingMeshPtrAt(size_t index) const;
    size_t GetNumOscillatingMeshes() const;
    const QuadTree<Mesh>& GetQuadTree() const;
    
private:
    
    int _currentCameraIndex = 0;
    std::vector<Camera> _cameraList;
    std::vector<std::shared_ptr<Mesh>> _meshList;
    std::vector<std::shared_ptr<Mesh>> _oscillatingMeshList;
    QuadTree<Mesh> _quadTree;
};
