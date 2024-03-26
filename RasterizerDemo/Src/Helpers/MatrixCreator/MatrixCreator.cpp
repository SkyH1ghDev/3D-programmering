#include "MatrixCreator.hpp"

DirectX::XMMATRIX MatrixCreator::CreateWorldMatrix(const float& angle)
{
	using namespace DirectX;
	XMMATRIX translationMatrix = XMMatrixTranslation(0, 0, 0);
	XMMATRIX rotationMatrix = XMMatrixRotationY(angle);
	return XMMatrixTranspose(XMMatrixMultiply(translationMatrix, rotationMatrix));
}

DirectX::XMMATRIX MatrixCreator::CreateWorldMatrix(const WorldMatrixConfig& worldMatrixConfig)
{
	using namespace DirectX;
	XMMATRIX translationMatrix = XMMatrixTranslation(0, 0, 0);
	XMMATRIX rotationMatrix = XMMatrixRotationY(worldMatrixConfig.InitialAngle);
	return XMMatrixTranspose(XMMatrixMultiply(translationMatrix, rotationMatrix));
}



DirectX::XMMATRIX MatrixCreator::CreateViewMatrix(const std::array<float, 4>& eyePos)
{
	using namespace DirectX;
	XMVECTOR eyePosition = XMVectorSet(eyePos[0], eyePos[1], eyePos[2], eyePos[3]);
	XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	return XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}

DirectX::XMMATRIX MatrixCreator::CreateViewMatrix(const ViewMatrixConfig& viewMatrixConfig)
{
	using namespace DirectX;
	XMVECTOR eyePosition = XMVectorSet(viewMatrixConfig.EyePos[0], viewMatrixConfig.EyePos[1], viewMatrixConfig.EyePos[2], viewMatrixConfig.EyePos[3]);
	XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	return XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
}



DirectX::XMMATRIX MatrixCreator::CreateProjectionMatrix(const float &fovAngle, const float &aspectRatio, const float &nearZ, const float &farZ)
{
	using namespace DirectX;
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngle), aspectRatio, nearZ, farZ);
}

DirectX::XMMATRIX MatrixCreator::CreateProjectionMatrix(const ProjectionMatrixConfig& projectionMatrixConfig)
{
	using namespace DirectX;
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(projectionMatrixConfig.FovAngle), projectionMatrixConfig.AspectRatio, projectionMatrixConfig.NearZ, projectionMatrixConfig.FarZ);
}
