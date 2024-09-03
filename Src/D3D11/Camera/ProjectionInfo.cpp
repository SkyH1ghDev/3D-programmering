#include "ProjectionInfo.hpp"
#include "WindowConfig.hpp"

ProjectionInfo::ProjectionInfo()
{
   WindowConfig windowConfig;
   
   this->AspectRatio = static_cast<float>(windowConfig.GetWidth()) / static_cast<float>(windowConfig.GetHeight());
   this->FovAngleY = 59.0f;
   this->NearZ = 0.1f;
   this->FarZ = 100.0f;
}

ProjectionInfo::ProjectionInfo(const ProjectionMatrixConfig &config)
{
   this->AspectRatio = config.GetAspectRatio();
   this->FovAngleY = config.GetFovAngle();
   this->NearZ = config.GetNearZ();
   this->FarZ = config.GetFarZ();
}
