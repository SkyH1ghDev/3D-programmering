#include "MatrixCreator.hpp"
#include "ViewMatrixConfig.hpp"

namespace DX = DirectX;


DX::XMMATRIX MatrixCreator::CreateWorldMatrix(const DX::XMFLOAT4& position) 
{
	DX::XMMATRIX worldXMMatrix = DX::XMMatrixTranslation(position.x, position.y, position.z);
	return DX::XMMatrixTranspose(worldXMMatrix);
}

DX::XMMATRIX MatrixCreator::CreateViewMatrix(const DX::XMFLOAT4& cameraPosition, const DX::XMFLOAT4& viewDirection, const DX::XMFLOAT4& upDirection)
{
	DX::XMVECTOR camPosition = DX::XMLoadFloat4(&cameraPosition);
	DX::XMVECTOR viewVector = DX::XMVectorAdd(camPosition, DX::XMLoadFloat4(&viewDirection));
	DX::XMVECTOR upVector = DX::XMLoadFloat4(&upDirection);
	
	return DX::XMMatrixLookAtLH(camPosition, viewVector, upVector);

}



DX::XMMATRIX MatrixCreator::CreateProjectionMatrix(const ProjectionInfo& projInfo)
{
	float fovAngle = DX::XMConvertToRadians(projInfo.FovAngleY);
	float aspectRatio = projInfo.AspectRatio;
	float nearZ = projInfo.NearZ;
	float farZ = projInfo.FarZ;
	
	return DX::XMMatrixPerspectiveFovLH(fovAngle, aspectRatio, nearZ , farZ);
}

DX::XMMATRIX MatrixCreator::CreateViewProjectionMatrix(const Camera& camera)
{
	DX::XMMATRIX viewMatrix = CreateViewMatrix(camera.GetPosition(), camera.GetForward(), camera.GetUp());
	DX::XMMATRIX projectionMatrix = CreateProjectionMatrix(camera.GetProjectionInfo());

	return DX::XMMatrixMultiplyTranspose(viewMatrix, projectionMatrix);
}


DX::XMMATRIX MatrixCreator::CreateTranslationMatrix(const float& x, const float& y, const float& z)
{
	DX::XMMATRIX translationMatrix = DX::XMMatrixTranslation(x, y, z);
	return DX::XMMatrixTranspose(translationMatrix);
}

DX::XMMATRIX MatrixCreator::CreateRotationMatrixX(const float& yaw)
{
	return DX::XMMatrixTranspose(DX::XMMatrixRotationX(yaw));
}

DX::XMMATRIX MatrixCreator::CreateRotationMatrixY(const float& pitch)
{
	return DX::XMMatrixTranspose(DX::XMMatrixRotationY(pitch));
}

DX::XMMATRIX MatrixCreator::CreateRotationMatrixZ(const float& roll)
{
	return DX::XMMatrixTranspose(DX::XMMatrixRotationZ(roll));
}

DX::XMMATRIX MatrixCreator::CreateRotationMatrixAxis(const float& angle, const DX::XMVECTOR& axis)
{
	return DX::XMMatrixRotationAxis(axis, angle);
}



