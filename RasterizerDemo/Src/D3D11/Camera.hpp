#pragma once

#define DX DirectX

#include <d3d11_4.h>
#include <DirectXMath.h>

#include "ConstantBuffer.hpp"

struct ProjectionInfo
{
	float fovAngleY = 0.0f;
	float aspectRatio = 0.0f;
	float nearZ = 0.0f;
	float farZ = 0.0f;
};

class Camera
{
private:
	DX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT3 forward = { 0.0f, 0.0f, 1.0f };
	DX::XMFLOAT3 right = { 1.0f, 0.0f, 0.0f };
	DX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
	ProjectionInfo projInfo;

	ConstantBuffer cameraBuffer;

	void MoveInDirection(float amount, const DX::XMFLOAT3& direction);
	void RotateAroundAxis(float amount, const DX::XMFLOAT3& axis);

public:
	Camera() = default;
	Camera(ID3D11Device* device, const ProjectionInfo& projectionInfo,
		const DX::XMFLOAT3& initialPosition = DX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	~Camera() = default;
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;
	Camera(Camera&& other) = default;
	Camera& operator=(Camera&& other) = default;

	void Initialize(ID3D11Device* device, const ProjectionInfo& projectionInfo,
		const DX::XMFLOAT3& initialPosition = DX::XMFLOAT3(0.0f, 0.0f, 0.0f));

	void MoveForward(float amount);
	void MoveRight(float amount);
	void MoveUp(float amount);

	void RotateForward(float amount);
	void RotateRight(float amount);
	void RotateUp(float amount);

	const DX::XMFLOAT3& GetPosition() const;
	const DX::XMFLOAT3& GetForward() const;
	const DX::XMFLOAT3& GetRight() const;
	const DX::XMFLOAT3& GetUp() const;

	void UpdateInternalConstantBuffer(ID3D11DeviceContext* context);
	ID3D11Buffer* GetConstantBuffer() const;

	DirectX::XMFLOAT4X4 GetViewProjectionMatrix() const;
};