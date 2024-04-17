#pragma once

#include <d3d11_4.h>

class BufferFlagData
{
public:
   D3D11_USAGE Usage;
   D3D11_CPU_ACCESS_FLAG CpuAccess;
   D3D11_RESOURCE_MISC_FLAG Misc;

   BufferFlagData();
};
