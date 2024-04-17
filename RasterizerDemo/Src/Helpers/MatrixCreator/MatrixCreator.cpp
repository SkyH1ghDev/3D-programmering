#define DX DirectX

#include "MatrixCreator.hpp"

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



DX::XMMATRIX MatrixCreator::CreateViewXMMATRIX(const std::array<float, 4>& camPos, const std::array<float, 4>& focalPoint)
{
	DX::XMVECTOR eyePosition = DX::XMVectorSet(camPos[0], camPos[1], camPos[2], camPos[3]);
	DX::XMVECTOR focusPosition = DX::XMVectorSet(focalPoint[0], focalPoint[1],focalPoint[2], focalPoint[3]);
	DX::XMVECTOR upDirection = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	return DX::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}

DX::XMMATRIX MatrixCreator::CreateViewXMMATRIX()
{
	ViewMatrixConfig viewMatrixConfig;
	
	DX::XMVECTOR camPosition = DX::XMVectorSet(viewMatrixConfig.GetCamPosition()[0], viewMatrixConfig.GetCamPosition()[1], viewMatrixConfig.GetCamPosition()[2], viewMatrixConfig.GetCamPosition()[3]);
	DX::XMVECTOR focusPosition = DX::XMVectorSet(viewMatrixConfig.GetFocusPosition()[0], viewMatrixConfig.GetFocusPosition()[1], viewMatrixConfig.GetFocusPosition()[2], viewMatrixConfig.GetFocusPosition()[3]);
	DX::XMVECTOR upDirection = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	return DX::XMMatrixLookAtLH(camPosition, focusPosition, upDirection);
}



DX::XMMATRIX MatrixCreator::CreateProjectionXMMATRIX(const float &fovAngle, const float &aspectRatio, const float &nearZ, const float &farZ)
{
	return DX::XMMatrixPerspectiveFovLH(DX::XMConvertToRadians(fovAngle), aspectRatio, nearZ, farZ);
}

DX::XMMATRIX MatrixCreator::CreateProjectionXMMATRIX(const ProjectionMatrixConfig& projectionMatrixConfig)
{
	return DX::XMMatrixPerspectiveFovLH(DX::XMConvertToRadians(projectionMatrixConfig.GetFovAngle()), projectionMatrixConfig.GetAspectRatio(), projectionMatrixConfig.GetNearZ(), projectionMatrixConfig.GetFarZ());
}
