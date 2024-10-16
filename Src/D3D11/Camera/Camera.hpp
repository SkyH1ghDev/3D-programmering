#pragma once

#include <d3d11_4.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "ProjectionInfo.hpp"
#include "DepthBuffer.hpp"

namespace DX = DirectX;

class Camera
{
public:
	Camera() = delete;
	Camera(HRESULT& hr, ID3D11Device* device, const ProjectionInfo& projectionInfo, const DX::XMFLOAT4& initialPosition);
	
	~Camera() = default;
	Camera(const Camera& other) = default;
	Camera& operator=(const Camera& other) = default;
	Camera(Camera&& other) = default;
	Camera& operator=(Camera&& other) = default;

	void Initialize(ID3D11Device* device,
		const ProjectionInfo& projectionInfo,
		const DX::XMFLOAT3& initialPosition = DX::XMFLOAT3(0.0f, 0.0f, 0.0f));

	void MoveForward(const float& amount, const float& deltaTime = 1);
	void MoveBackward(const float& amount, const float& deltaTime = 1);
	void MoveLeft(const float& amount, const float& deltaTime = 1);
	void MoveRight(const float& amount, const float& deltaTime = 1);
	void MoveUp(const float& amount, const float& deltaTime = 1);
	void MoveDown(const float& amount, const float& deltaTime = 1);

	void ApplyRotation(const float& deltaPitch, const float& deltaYaw);
	
	DX::BoundingFrustum GetBoundingFrustum() const;
	void SetBoundingFrustum(const DX::BoundingFrustum& boundingFrustum);
	
	const DX::XMFLOAT4& GetPosition() const;
	const DX::XMFLOAT4& GetForward() const;
	const DX::XMFLOAT4& GetRight() const;
	const DX::XMFLOAT4& GetUp() const;

	const ProjectionInfo& GetProjectionInfo() const;
	DepthBuffer GetDepthBuffer() const;
	
private:
	void MoveInDirection(const float& amount, const DX::XMFLOAT4& direction);

private:
	DX::XMFLOAT4 _position = { 0.0f, 0.0f, 0.0f, 1.0f };
	ProjectionInfo _projInfo;

	DX::BoundingFrustum _boundingFrustum;

	DX::XMFLOAT4 _forward = { 0.0f, 0.0f, 1.0f, 0.0f };
	DX::XMFLOAT4 _right = { 1.0f, 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT4 _up = { 0.0f, 1.0f, 0.0f, 0.0f };

	float _pitch = 0.0f;
	float _yaw = 0.0f;
	
	DepthBuffer _depthBuffer;
};