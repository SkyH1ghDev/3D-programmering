#pragma once

#include <d3d11_4.h>
#include <DirectXMath.h>
#include "ConstantBuffer.hpp"
#include "ProjectionInfo.hpp"
#include "ViewMatrixConfig.hpp"

namespace DX = DirectX;

class Camera
{
public:
	Camera() = delete;
	Camera(HRESULT& hr, ID3D11Device* device,
		const DX::XMFLOAT4& initialPosition = ViewMatrixConfig().GetCamPosition(),
		const ProjectionInfo& projectionInfo = ProjectionInfo(ProjectionMatrixConfig()));
	
	~Camera() = default;
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;
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

	void RotateForward(float amount);
	void RotateRight(float amount);
	void RotateUp(float amount);

	const DX::XMFLOAT4& GetPosition() const;
	const DX::XMFLOAT3& GetForward() const;
	const DX::XMFLOAT3& GetRight() const;
	const DX::XMFLOAT3& GetUp() const;

	void UpdateInternalConstantBuffer(ID3D11DeviceContext* context);
	ID3D11Buffer* GetConstantBuffer() const;
	
	DX::XMFLOAT4X4 GetViewProjectionMatrix() const;
	
private:

	// Reminder: _position and _projInfo need to be declared before _viewProjectionMatrix due to how the initialization of Camera works
	DX::XMFLOAT4 _position = { 0.0f, 0.0f, 0.0f, 1.0f };
	DX::XMFLOAT4 _directionVector = { 0.0f, 0.0f, 1.0f, 0.0f };
	ProjectionInfo _projInfo;
	
	const DX::XMFLOAT3 _forward = { 0.0f, 0.0f, 1.0f };
	const DX::XMFLOAT3 _backward = { 0.0f, 0.0f, -1.0f };
	const DX::XMFLOAT3 _right = { 1.0f, 0.0f, 0.0f };
	const DX::XMFLOAT3 _left = { -1.0f, 0.0f, 0.0f }; 
	const DX::XMFLOAT3 _up = { 0.0f, 1.0f, 0.0f };
	const DX::XMFLOAT3 _down = { 0.0f, -1.0f, 0.0f };
	
	DX::XMFLOAT4X4 _viewProjectionMatrix;
	
	ConstantBuffer _cameraBuffer;

	void MoveInDirection(const float& amount, const DX::XMFLOAT3& direction);
	void RotateAroundAxis(const float& amount, const DX::XMFLOAT3& axis);

	BufferFlagData GetBufferFlags();
	DX::XMFLOAT4X4 CreateViewProjectionMatrix(const DX::XMFLOAT4 &position, const DX::XMFLOAT4 &directionVector, const ProjectionInfo& projInfo) const;
	DX::XMFLOAT4X4 CreateViewProjectionMatrix() const;
};