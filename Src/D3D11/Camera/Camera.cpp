#include "Camera.hpp"
#include "MatrixCreator.hpp"
#include <algorithm>
#include <iostream>

// Overloading float3 mult as vector mult
DX::XMFLOAT4 operator*(const DX::XMFLOAT4& a, const float& b)
{
	return {a.x * b, a.y * b, a.z * b, a.w};
}

DX::XMFLOAT4 operator*(const float& a, const DX::XMFLOAT4& b)
{
	return operator*(b, a);
}

Camera::Camera(HRESULT& hr, ID3D11Device* device, const ProjectionInfo& projInfo, const DX::XMFLOAT4& initialPosition):
	_depthBuffer(DepthBuffer(hr, device))
{
	MatrixCreator matrixCreator;
	
	this->_position = initialPosition;
	this->_projInfo = projInfo;
	this->_boundingFrustum = DX::BoundingFrustum(matrixCreator.CreateViewProjectionMatrix(*this));
}

void Camera::ApplyRotation(const float& deltaPitch, const float& deltaYaw)
{
	this->_pitch = std::max<float>(-DX::XM_PIDIV2, std::min<float>(DX::XM_PIDIV2, this->_pitch + deltaPitch));
	this->_yaw = std::fmod(this->_yaw + deltaYaw, 2 * DX::XM_PI);
	
	DX::XMVECTOR quaternion = DX::XMQuaternionRotationRollPitchYaw(this->_pitch, this->_yaw, 0.0f);
	DX::XMVECTOR quaternionConjugate = DX::XMQuaternionConjugate(quaternion);
	
	DX::XMVECTOR newForward = DX::XMQuaternionMultiply(DX::XMQuaternionMultiply(quaternionConjugate, {0.0f, 0.0f, 1.0f, 0.0f}), quaternion);
	newForward = DX::XMVectorSetW(newForward, 0.0f);
	newForward = DX::XMVector4Normalize(newForward);
	
	DX::XMVECTOR newRight = DX::XMQuaternionMultiply(DX::XMQuaternionMultiply(quaternionConjugate, {1.0f, 0.0f, 0.0f, 0.0f}), quaternion);
	newRight = DX::XMVectorSetW(newRight, 0.0f);
	newRight = DX::XMVector4Normalize(newRight);
	
	DX::XMVECTOR newUp = DX::XMQuaternionMultiply(DX::XMQuaternionMultiply(quaternionConjugate, {0.0f, 1.0f, 0.0f, 0.0f}), quaternion);
	newUp = DX::XMVectorSetW(newUp, 0.0f);
	newUp = DX::XMVector4Normalize(newUp);

	DX::XMStoreFloat4(&this->_forward, newForward);
	DX::XMStoreFloat4(&this->_up, newUp);
	DX::XMStoreFloat4(&this->_right, newRight);
}

DX::BoundingFrustum Camera::GetBoundingFrustum() const
{
	return this->_boundingFrustum;
}

void Camera::SetBoundingFrustum(const DX::BoundingFrustum& boundingFrustum)
{
	this->_boundingFrustum = boundingFrustum;
	this->_boundingFrustum.Origin = {this->_position.x, this->_position.y, this->_position.z};
	DX::XMVECTOR quaternionXMVector = DX::XMQuaternionRotationRollPitchYaw(this->_pitch, this->_yaw, 0.0f);
	DX::XMFLOAT4 quaternionFloat4;
	DX::XMStoreFloat4(&quaternionFloat4, quaternionXMVector);
	this->_boundingFrustum.Orientation = quaternionFloat4;
}

void Camera::MoveInDirection(const float& amount, const DX::XMFLOAT4& direction)
{
	DX::XMVECTOR directionXMVector = XMLoadFloat4(&direction);

	directionXMVector = DX::XMVectorScale(directionXMVector, amount);

	DX::XMFLOAT4 scaledDirectionXMFloat4;
	XMStoreFloat4(&scaledDirectionXMFloat4, directionXMVector);

	this->_position.x += scaledDirectionXMFloat4.x;
	this->_position.y += scaledDirectionXMFloat4.y;
	this->_position.z += scaledDirectionXMFloat4.z;
	this->_position.w = 1.0f;
}

void Camera::MoveForward(const float& amount, const float& deltaTime)
{
	MoveInDirection(amount * deltaTime, this->_forward);
}

void Camera::MoveBackward(const float& amount, const float& deltaTime)
{
	MoveInDirection(amount * deltaTime, this->_forward * -1);
}

void Camera::MoveLeft(const float& amount, const float& deltaTime)
{
	MoveInDirection(amount * deltaTime, this->_right * -1);
}

void Camera::MoveRight(const float &amount, const float& deltaTime)
{
	MoveInDirection(amount * deltaTime, this->_right);
}

void Camera::MoveUp(const float &amount, const float& deltaTime)
{
	MoveInDirection(amount * deltaTime, {0.0f, 1.0f, 0.0f, 0.0f});
}

void Camera::MoveDown(const float &amount, const float& deltaTime)
{
	MoveInDirection(amount * deltaTime, {0.0f, -1.0f, 0.0f, 0.0f});
}

DepthBuffer Camera::GetDepthBuffer() const
{
	return this->_depthBuffer;
}

const ProjectionInfo& Camera::GetProjectionInfo() const
{
	return this->_projInfo;
}

const DX::XMFLOAT4 &Camera::GetPosition() const
{
	return this->_position;
}

const DX::XMFLOAT4& Camera::GetForward() const
{
	return this->_forward;
}

const DX::XMFLOAT4& Camera::GetUp() const
{
	return this->_up;
}


