#pragma once
#include "IConfigBase.hpp"

enum RenderMode
{
   Forward,
   Deferred
};

class RenderConfig : public IConfigBase
{
public:
   RenderMode GetRenderMode() const { return this->_renderMode; }
   
private:
   RenderMode _renderMode = Forward;
};