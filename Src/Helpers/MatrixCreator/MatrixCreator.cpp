#include "MatrixCreator.hpp"
#include "ViewMatrixConfig.hpp"

namespace DX = DirectX;


DX::XMMATRIX MatrixCreator::CreateInitialWorldMatrixTransposed()
{
	DX::XMMATRIX worldXMMatrix = DX::XMMatrixTranslation(0, 0, 0);
	return DX::XMMatrixTranspose(worldXMMatrix);
}

DX::XMMATRIX MatrixCreator::CreateInitialViewMatrixTransposed()
{
	ViewMatrixConfig viewMatrixConfig;

	DX::XMFLOAT4 eyePositionXMFloat4 = viewMatrixConfig.GetCamPosition();
	DX::XMFLOAT4 directionVectorXMFloat4 = viewMatrixConfig.GetDirectionVector();
	
	DX::XMVECTOR eyePosition = DX::XMLoadFloat4(&eyePositionXMFloat4);
	DX::XMVECTOR focusPosition = DX::XMVectorAdd(eyePosition, DX::XMLoadFloat4(&directionVectorXMFloat4));
	DX::XMVECTOR upDirection = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	DX::XMMATRIX viewXMMatrix = DX::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	return DX::XMMatrixTranspose(viewXMMatrix);
}



DX::XMMATRIX MatrixCreator::CreateInitialProjectionMatrix()
{
	ProjectionMatrixConfig projectionMatrixConfig;

	float fovAngle = DX::XMConvertToRadians(DX::XMConvertToRadians(projectionMatrixConfig.GetFovAngle()));
	float aspectRatio = projectionMatrixConfig.GetAspectRatio();
	float nearZ = projectionMatrixConfig.GetNearZ();
	float farZ = projectionMatrixConfig.GetFarZ();
	
	return DX::XMMatrixPerspectiveFovLH(fovAngle, aspectRatio, nearZ , farZ);
}

DX::XMMATRIX MatrixCreator::CreateInitialViewProjectionMatrixTransposed()
{
	DX::XMMATRIX viewMatrix = CreateInitialViewMatrixTransposed();
	DX::XMMATRIX projectionMatrix = CreateInitialProjectionMatrix();

	return DX::XMMatrixMultiplyTranspose(viewMatrix, projectionMatrix);
}


DX::XMMATRIX MatrixCreator::CreateTranslationMatrix(const float& x, const float& y, const float& z)
{
	DX::XMMATRIX translationMatrix = DX::XMMatrixTranslation(x, y, z);
	return DX::XMMatrixTranspose(translationMatrix);
}
