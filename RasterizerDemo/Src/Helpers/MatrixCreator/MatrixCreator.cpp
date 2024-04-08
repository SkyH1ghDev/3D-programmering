#define DX DirectX

#include "MatrixCreator.hpp"

DirectX::XMMATRIX MatrixCreator::CreateWorldMatrix(const float& angle)
{
	DX::XMMATRIX translationMatrix = DX::XMMatrixTranslation(0, 0, 0);
	DX::XMMATRIX rotationMatrix = DX::XMMatrixRotationY(angle);
	return XMMatrixTranspose(XMMatrixMultiply(translationMatrix, rotationMatrix));
}

DirectX::XMMATRIX MatrixCreator::CreateWorldMatrix(const WorldMatrixConfig& worldMatrixConfig)
{
	DX::XMMATRIX translationMatrix = DX::XMMatrixTranslation(0, 0, 0);
	DX::XMMATRIX rotationMatrix = DX::XMMatrixRotationY(worldMatrixConfig.GetInitialAngle());
	return XMMatrixTranspose(XMMatrixMultiply(translationMatrix, rotationMatrix));
}



DirectX::XMMATRIX MatrixCreator::CreateViewMatrix(const std::array<float, 4>& eyePos)
{
	DX::XMVECTOR eyePosition = DX::XMVectorSet(eyePos[0], eyePos[1], eyePos[2], eyePos[3]);
	DX::XMVECTOR focusPosition = DX::XMVectorSet(0.0f + eyePos[0], 0.0f + eyePos[1], 0.0f + eyePos[2], 1.0f);
	DX::XMVECTOR upDirection = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	return DX::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}

DirectX::XMMATRIX MatrixCreator::CreateViewMatrix(const ViewMatrixConfig& viewMatrixConfig)
{
	DX::XMVECTOR eyePosition = DX::XMVectorSet(viewMatrixConfig.GetEyePosition()[0], viewMatrixConfig.GetEyePosition()[1], viewMatrixConfig.GetEyePosition()[2], viewMatrixConfig.GetEyePosition()[3]);
	DX::XMVECTOR focusPosition = DX::XMVectorSet(0.0f + viewMatrixConfig.GetEyePosition()[0], 0.0f + viewMatrixConfig.GetEyePosition()[1], 0.0f + viewMatrixConfig.GetEyePosition()[3], 1.0f);
	DX::XMVECTOR upDirection = DX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	return DX::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}



DirectX::XMMATRIX MatrixCreator::CreateProjectionMatrix(const float &fovAngle, const float &aspectRatio, const float &nearZ, const float &farZ)
{
	return DX::XMMatrixPerspectiveFovLH(DX::XMConvertToRadians(fovAngle), aspectRatio, nearZ, farZ);
}

DirectX::XMMATRIX MatrixCreator::CreateProjectionMatrix(const ProjectionMatrixConfig& projectionMatrixConfig)
{
	return DX::XMMatrixPerspectiveFovLH(DX::XMConvertToRadians(projectionMatrixConfig.GetFovAngle()), projectionMatrixConfig.GetAspectRatio(), projectionMatrixConfig.GetNearZ(), projectionMatrixConfig.GetFarZ());
}
