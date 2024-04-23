#include "Camera.hpp"
#include "MatrixCreator.hpp"

Camera::Camera(HRESULT& hr, ID3D11Device* device, const DX::XMFLOAT4& initialPosition, const ProjectionInfo& projectionInfo):
	_projInfo(projectionInfo),
	_position(initialPosition),
	_viewProjectionMatrix(CreateViewProjectionMatrix(this->_position, this->_directionVector, this->_projInfo)),
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

void Camera::MoveInDirection(const float& amount, const DX::XMFLOAT3& direction)
{
	DX::XMVECTOR directionXMVector = XMLoadFloat3(&direction);

	directionXMVector = DX::XMVectorScale(directionXMVector, amount);

	DX::XMFLOAT3 scaledDirectionXMFloat3;
	XMStoreFloat3(&scaledDirectionXMFloat3, directionXMVector);

	this->_position.x += scaledDirectionXMFloat3.x;
	this->_position.y += scaledDirectionXMFloat3.y;
	this->_position.z += scaledDirectionXMFloat3.z;

	this->_viewProjectionMatrix = CreateViewProjectionMatrix(this->_position, this->_directionVector, this->_projInfo);
}

void Camera::MoveForward(const float& amount, const float& deltaTime)
{
	MoveInDirection(amount * deltaTime, this->_forward);
}

void Camera::MoveBackward(const float& amount, const float& deltaTime)
{
	MoveInDirection(amount * deltaTime, this->_backward);
}

void Camera::MoveLeft(const float& amount, const float& deltaTime)
{
	MoveInDirection(amount * deltaTime, this->_left);
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
	MoveInDirection(amount * deltaTime, this->_down);
}




