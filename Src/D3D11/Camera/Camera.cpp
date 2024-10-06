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
	this->_position = initialPosition;
	this->_projInfo = projInfo;
}

void Camera::ApplyRotation(const float& deltaPitch, const float& deltaYaw)
{
	MatrixCreator matrixCreator;
	DX::XMMATRIX yawRotationMatrix = matrixCreator.CreateRotationMatrixAxis(this->_yaw, {0.0f, 1.0f, 0.0f, 0.0f});
	DX::XMVECTOR leftAxis = DX::XMVectorScale(DX::XMLoadFloat4(&this->_right), -1);
	DX::XMMATRIX pitchRotationMatrix = matrixCreator.CreateRotationMatrixAxis(this->_pitch, leftAxis);

	this->_pitch = std::max<float>(-DX::XM_PI, std::min<float>(DX::XM_PI, this->_pitch + deltaPitch));
	this->_yaw = std::fmod(this->_yaw + deltaYaw, 2 * DX::XM_PI);
	
	DX::XMMATRIX pitchYawRotationMatrix = DX::XMMatrixMultiply(yawRotationMatrix, pitchRotationMatrix);

	DX::XMVECTOR quaternion = DX::XMQuaternionRotationMatrix(pitchYawRotationMatrix);
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
	
	// Calculate new forward vector
	/*DX::XMVECTOR newForward = DX::XMVector3Transform({0, 0, 1}, pitchYawRotationMatrix);
	newForward = DX::XMVector3Normalize(newForward);
	
	DX::XMVECTOR newRight = DX::XMVector3Transform({1, 0, 0}, pitchYawRotationMatrix);
	newRight = DX::XMVector3Normalize(newRight);

	DX::XMVECTOR newUp = DX::XMVector3Transform({0, 1, 0}, pitchYawRotationMatrix);
	newUp = DX::XMVector3Normalize(newUp);*/
	
	DX::XMStoreFloat4(&this->_forward, newForward);
	DX::XMStoreFloat4(&this->_up, newUp);
	DX::XMStoreFloat4(&this->_right, newRight);

	std::cout << "Pitch: " << this->_pitch << std::endl;
	std::cout << "Yaw: " << this->_yaw << std::endl;

	std::cout << "Forward Vector: (" << this->_forward.x << ", " << this->_forward.y << ", " << this->_forward.z << ")" << std::endl;
	std::cout << "Up Vector: (" << this->_up.x << ", " << this->_up.y << ", " << this->_up.z << ")" << std::endl;
	std::cout << "Right Vector: (" << this->_right.x << ", " << this->_right.y << ", " << this->_right.z << ")" << std::endl;
}


void Camera::RotateAroundAxis(const float& amount, const DX::XMFLOAT4& axis)
{
	
	/*DX::XMVECTOR axisVector = DX::XMLoadFloat4(&axis);
	DX::XMMATRIX rotationMatrix = DX::XMMatrixRotationAxis(axisVector, amount);
	
	DX::XMVECTOR forwardVector = DX::XMLoadFloat4(&this->_forward); 
	DX::XMVECTOR rightVector = DX::XMLoadFloat4(&this->_right); 
	DX::XMVECTOR upVector = DX::XMLoadFloat4(&this->_up);

	forwardVector = DX::XMVector4Normalize(DX::XMVector4Transform(forwardVector, rotationMatrix));
	rightVector = DX::XMVector4Normalize(DX::XMVector4Transform(rightVector, rotationMatrix));
	upVector = DX::XMVector4Normalize(DX::XMVector4Transform(upVector, rotationMatrix));

	// Orthogonalize right vector with forward vector
	DX::XMVECTOR rightProjectionVector = DX::XMVectorMultiply(DX::XMVector4Dot(rightVector, forwardVector), forwardVector);
	rightVector = DX::XMVector4Normalize(DX::XMVectorSubtract(rightVector, rightProjectionVector));

	// Orthogonalize up vector with forward vector
	DX::XMVECTOR upProjectionVector = DX::XMVectorMultiply(DX::XMVector4Dot(upVector, forwardVector), forwardVector);
	upVector = DX::XMVectorSubtract(upVector, upProjectionVector); // No need to normalize until all calculations are finished

	// Orthogonalize up vector with right vector
	upProjectionVector = DX::XMVectorMultiply(DX::XMVector4Dot(upVector, rightVector), rightVector);
	upVector = DX::XMVector4Normalize(DX::XMVectorSubtract(upVector, upProjectionVector));

	DX::XMStoreFloat4(&this->_forward, forwardVector);
	DX::XMStoreFloat4(&this->_right, rightVector);
	DX::XMStoreFloat4(&this->_up, upVector);
    */
	//MatrixCreator matrixCreator;
	//this->_viewProjectionMatrix = matrixCreator.CreateViewProjectionMatrix(*this);
}

void Camera::RotateForward(float amount, const float &deltaTime)
{
	RotateAroundAxis(amount * deltaTime, this->_forward);
}

void Camera::RotateBackward(float amount, const float &deltaTime)
{
	RotateAroundAxis(amount * deltaTime * -1, this->_forward); 
}

void Camera::RotateRight(float amount, const float &deltaTime)
{
	RotateAroundAxis(amount * deltaTime, this->_right);
}

void Camera::RotateLeft(float amount, const float &deltaTime)
{
	RotateAroundAxis(amount * deltaTime * -1, this->_right); 
}

void Camera::RotateUp(float amount, const float &deltaTime)
{
	RotateAroundAxis(amount * deltaTime, {0.0f, 1.0f, 0.0f, 0.0f});
}

void Camera::RotateDown(float amount, const float &deltaTime)
{
	RotateAroundAxis(amount * deltaTime * -1, {0.0f, 1.0f, 0.0f, 0.0f});
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
	MoveInDirection(amount * deltaTime, this->_up);
}

void Camera::MoveDown(const float &amount, const float& deltaTime)
{
	MoveInDirection(amount * deltaTime, this->_up * -1);
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


