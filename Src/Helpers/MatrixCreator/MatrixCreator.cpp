#include "MatrixCreator.hpp"
#include "WorldMatrixConfig.hpp"
#include "ViewMatrixConfig.hpp"

namespace DX = DirectX;

DX::XMMATRIX MatrixCreator::CreateWorldXMMATRIX(const float& angle)
{
	DX::XMMATRIX translationMatrix = DX::XMMatrixTranslation(0, 0, 0);
	DX::XMMATRIX rotationMatrix = DX::XMMatrixRotationY(angle);
	return XMMatrixTranspose(XMMatrixMultiply(translationMatrix, rotationMatrix));
}

DX::XMMATRIX MatrixCreator::CreateWorldXMMATRIX()
{
	WorldMatrixConfig worldMatrixConfig;
	
	DX::XMMATRIX translationMatrix = DX::XMMatrixTranslation(0, 0, 0);
	DX::XMMATRIX rotationMatrix = DX::XMMatrixRotationY(worldMatrixConfig.GetInitialAngle());
	return XMMatrixTranspose(XMMatrixMultiply(translationMatrix, rotationMatrix));
}



DX::XMFLOAT4X4 MatrixCreator::CreateWorldXMFLOAT4X4(const float &angle)
{
	DX::XMMATRIX worldXMMatrix = CreateWorldXMMATRIX(angle);

	DX::XMFLOAT4X4 worldFloat4X4;
	DX::XMStoreFloat4x4(&worldFloat4X4, worldXMMatrix);

	return worldFloat4X4;
}

DX::XMFLOAT4X4 MatrixCreator::CreateWorldXMFLOAT4X4()
{
	DX::XMMATRIX worldXMMatrix = CreateWorldXMMATRIX();

	DX::XMFLOAT4X4 worldFloat4X4;
	DX::XMStoreFloat4x4(&worldFloat4X4, worldXMMatrix);

	return worldFloat4X4;
}



DX::XMMATRIX MatrixCreator::CreateViewXMMATRIX(const DX::XMFLOAT4 &camPos, const DX::XMFLOAT4 &directionVector)
{
	DX::XMVECTOR eyePosition = DX::XMLoadFloat4(&camPos);
	DX::XMVECTOR focusPosition = DX::XMVectorAdd(eyePosition, XMLoadFloat4(&directionVector));
	DX::XMVECTOR upDirection = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	return DX::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}

DX::XMMATRIX MatrixCreator::CreateViewXMMATRIX()
{
	ViewMatrixConfig viewMatrixConfig;

	DX::XMFLOAT4 eyePositionXMFloat4 = viewMatrixConfig.GetCamPosition();
	DX::XMFLOAT4 directionVectorXMFloat4 = viewMatrixConfig.GetDirectionVector();
	
	DX::XMVECTOR eyePosition = DX::XMLoadFloat4(&eyePositionXMFloat4);
	DX::XMVECTOR focusPosition = DX::XMVectorAdd(eyePosition, DX::XMLoadFloat4(&directionVectorXMFloat4));
	DX::XMVECTOR upDirection = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	return DX::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}



DX::XMMATRIX MatrixCreator::CreateProjectionXMMATRIX(const float &fovAngle, const float &aspectRatio, const float &nearZ, const float &farZ)
{
	return DX::XMMatrixPerspectiveFovLH(DX::XMConvertToRadians(fovAngle), aspectRatio, nearZ, farZ);
}

DX::XMMATRIX MatrixCreator::CreateProjectionXMMATRIX(const ProjectionMatrixConfig& projectionMatrixConfig)
{
	return DX::XMMatrixPerspectiveFovLH(DX::XMConvertToRadians(projectionMatrixConfig.GetFovAngle()), projectionMatrixConfig.GetAspectRatio(), projectionMatrixConfig.GetNearZ(), projectionMatrixConfig.GetFarZ());
}


DX::XMFLOAT4X4 MatrixCreator::CreateTranslationMatrixXMFLOAT4X4(const float& x, const float& y, const float& z)
{
	DX::XMMATRIX translationMatrix = DX::XMMatrixTranslation(x, y, z);
	DX::XMMATRIX transposedTranslationMatrix = DX::XMMatrixTranspose(translationMatrix);

	DX::XMFLOAT4X4 translationMatrixXMFLOAT4X4;

	DX::XMStoreFloat4x4(&translationMatrixXMFLOAT4X4, transposedTranslationMatrix);
	return translationMatrixXMFLOAT4X4;
}
