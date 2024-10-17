#include "SpotLight.hpp"

#include "MatrixCreator.hpp"

SpotLight::SpotLight(HRESULT& hr, ID3D11Device* device, const ProjectionInfo& projectionInfo, const DX::XMFLOAT4& position, const DX::XMFLOAT4& lightColour, const float& angle) :
    _camera(Camera(hr, device, projectionInfo, position))
{
    this->_lightColour = lightColour;
    this->_angle = angle;
}

Camera& SpotLight::GetCamera()
{
    return this->_camera;
}

DX::XMFLOAT4 SpotLight::GetLightColour() const
{
    return this->_lightColour;
}

float SpotLight::GetAngle() const
{
    return this->_angle;
}

CSLightData2 SpotLight::GetSpotlightData()
{
    MatrixCreator matrixCreator;
    
    CSLightData2 spotlightData;
    spotlightData.angle = fabs(this->_angle);
    spotlightData.colour = this->_lightColour;
    spotlightData.direction = this->_camera.GetForward();
    spotlightData.position = this->_camera.GetPosition();

    DX::XMMATRIX viewProjMatrix;
    if (fmod(this->_angle, DX::XM_2PI) >= DX::XM_PI)
    {
        viewProjMatrix = matrixCreator.CreateOrthographicViewProjectionMatrix(this->_camera);    
    }
    else
    {
        viewProjMatrix = matrixCreator.CreateViewProjectionMatrix(this->_camera);
    }
    
    DX::XMFLOAT4X4 viewProjectionMatrix;
    DX::XMStoreFloat4x4(&viewProjectionMatrix, viewProjMatrix);

    spotlightData.vpMatrix = viewProjectionMatrix;

    return spotlightData;
}


