#pragma once
#include "DepthBuffer.hpp"

class ILight
{
protected:
    ILight();

    DepthBuffer _depthBuffer;
};
