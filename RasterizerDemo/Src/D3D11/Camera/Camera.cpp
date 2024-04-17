#include "Camera.hpp"
#include "MatrixCreator.hpp"

BufferFlagData Camera::GetBufferFlags()
{
	BufferFlagData camBufferFlags;
	camBufferFlags.Usage = D3D11_USAGE_DYNAMIC;
	camBufferFlags.CpuAccess = D3D11_CPU_ACCESS_WRITE;

	return camBufferFlags;
}

Camera::Camera(HRESULT& hr, ID3D11Device* device, const ProjectionInfo& projectionInfo, const DX::XMFLOAT3& initialPosition):
	_viewProjectionMatrix(CreateViewProjectionMatrix(initialPosition, projectionInfo)),
	_cameraBuffer(ConstantBuffer(hr, device, sizeof(this->_viewProjectionMatrix), &this->_viewProjectionMatrix, 0, 0, 0, GetBufferFlags()))
{
	this->_projInfo = projectionInfo;
	this->_position = initialPosition;
}

DX::XMFLOAT4X4 Camera::CreateViewProjectionMatrix(const DX::XMFLOAT3& position, const ProjectionInfo& projInfo) const
{
   MatrixCreator matrixCreator;

   const DX::XMMATRIX viewMatrix = matrixCreator.CreateViewXMMATRIX({position.x, position.y, position.z, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
   const DX::XMMATRIX projectionMatrix = matrixCreator.CreateProjectionXMMATRIX(projInfo.FovAngleY, projInfo.AspectRatio, projInfo.NearZ, projInfo.FarZ);

   const DX::XMMATRIX viewProjectionMatrix = DX::XMMatrixMultiplyTranspose(viewMatrix, projectionMatrix);

   DX::XMFLOAT4X4 viewProjMatrix4X4;
   DX::XMStoreFloat4x4(&viewProjMatrix4X4, viewProjectionMatrix);

   return viewProjMatrix4X4;
}

DX::XMFLOAT4X4 Camera::GetViewProjectionMatrix() const
{
	return this->_viewProjectionMatrix;
}