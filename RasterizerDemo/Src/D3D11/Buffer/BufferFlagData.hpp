#pragma once

#include <d3d11_4.h>

class BufferFlagData
{
public:
   D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
   D3D11_CPU_ACCESS_FLAG CpuAccess = static_cast<D3D11_CPU_ACCESS_FLAG>(0);
   D3D11_RESOURCE_MISC_FLAG Misc = static_cast<D3D11_RESOURCE_MISC_FLAG>(0);
};
