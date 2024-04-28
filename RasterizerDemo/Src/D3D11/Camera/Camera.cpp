#include "Camera.hpp"
#include "MatrixCreator.hpp"

// Overloading float3 mult as vector mult
DX::XMFLOAT4 operator*(const DX::XMFLOAT4& a, const float& b)
{
	return {a.x * b, a.y * b, a.z * b, a.w};
}

DX::XMFLOAT4 operator*(const float& a, const DX::XMFLOAT4& b)
{
	return operator*(b, a);
}

Camera::Camera(HRESULT& hr, ID3D11Device* device, const DX::XMFLOAT4& initialPosition, const ProjectionInfo& projectionInfo):
	_projInfo(projectionInfo),
	_position(initialPosition),
	_viewProjectionMatrix(CreateViewProjectionMatrix(this->_position, this->_forward, this->_projInfo)),
	_cameraBuffer(ConstantBuffer(hr, device, sizeof(this->_viewProjectionMatrix), &this->_viewProjectionMatrix, 0, 0, 0, GetBufferFlags()))
{
}

DX::XMFLOAT4X4 Camera::CreateViewProjectionMatrix(const DX::XMFLOAT4 &position, const DX::XMFLOAT4 &directionVector, const ProjectionInfo& projInfo) const
{
   MatrixCreator matrixCreator;

   const DX::XMMATRIX viewMatrix = matrixCreator.CreateViewXMMATRIX(position, directionVector);
   const DX::XMMATRIX projectionMatrix = matrixCreator.CreateProjectionXMMATRIX(projInfo.FovAngleY, projInfo.AspectRatio, projInfo.NearZ, projInfo.FarZ);

   const DX::XMMATRIX viewProjectionMatrix = XMMatrixMultiplyTranspose(viewMatrix, projectionMatrix);

   DX::XMFLOAT4X4 viewProjMatrix4x4;
   XMStoreFloat4x4(&viewProjMatrix4x4, viewProjectionMatrix);

   return viewProjMatrix4x4;
}

DX::XMFLOAT4X4 Camera::CreateViewProjectionMatrix() const
{
	MatrixCreator matrixCreator;
	ProjectionInfo projInfo = this->_projInfo;
	
	const DX::XMMATRIX viewXMMatrix = matrixCreator.CreateViewXMMATRIX();
	const DX::XMMATRIX projectionXMMatrix = matrixCreator.CreateProjectionXMMATRIX(projInfo.FovAngleY, projInfo.AspectRatio, projInfo.NearZ, projInfo.FarZ);

	const DX::XMMATRIX viewProjXMMatrix = XMMatrixMultiply(viewXMMatrix, projectionXMMatrix);

	DX::XMFLOAT4X4 viewProjMatrix4x4;
	XMStoreFloat4x4(&viewProjMatrix4x4, viewProjXMMatrix);

	return viewProjMatrix4x4;
}

void Camera::RotateAroundAxis(const float &amount, const DX::XMFLOAT4 &axis)
{
	DX::XMVECTOR axisVector = DX::XMLoadFloat4(&axis);
	DX::XMMATRIX rotationMatrix = DX::XMMatrixRotationAxis(axisVector, amount);
	
	DX::XMVECTOR forwardVector = DX::XMLoadFloat4(&this->_forward); 
	DX::XMVECTOR rightVector = DX::XMLoadFloat4(&this->_right); 
	DX::XMVECTOR upVector = DX::XMLoadFloat4(&this->_up);

	DX::XMStoreFloat4(&this->_forward, DX::XMVector4Normalize(DX::XMVector4Transform(forwardVector, rotationMatrix)));
	DX::XMStoreFloat4(&this->_right, DX::XMVector4Normalize(DX::XMVector4Transform(rightVector, rotationMatrix)));
	DX::XMStoreFloat4(&this->_up, DX::XMVector4Normalize(DX::XMVector4Transform(upVector, rotationMatrix)));

	this->_viewProjectionMatrix = CreateViewProjectionMatrix(this->_position, this->_forward, this->_projInfo);
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
	RotateAroundAxis(amount * deltaTime, this->_up);
}

void Camera::RotateDown(float amount, const float &deltaTime)
{
	RotateAroundAxis(amount * deltaTime * -1, this->_up);
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

	this->_viewProjectionMatrix = CreateViewProjectionMatrix(this->_position, this->_forward, this->_projInfo);
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

void Camera::UpdateInternalConstantBuffer(ID3D11DeviceContext *context)
{
	this->_cameraBuffer.UpdateBuffer(context, &this->_viewProjectionMatrix, sizeof(_viewProjectionMatrix));
}

ID3D11Buffer* Camera::GetConstantBuffer() const
{
	return this->_cameraBuffer.GetBuffer();
}


DX::XMFLOAT4X4 Camera::GetViewProjectionMatrix() const
{
	return this->_viewProjectionMatrix;
}

BufferFlagData Camera::GetBufferFlags()
{
	BufferFlagData camBufferFlags;
	camBufferFlags.Usage = D3D11_USAGE_DYNAMIC;
	camBufferFlags.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	return camBufferFlags;
}

const DX::XMFLOAT4 &Camera::GetPosition() const
{
	return this->_position;
}

