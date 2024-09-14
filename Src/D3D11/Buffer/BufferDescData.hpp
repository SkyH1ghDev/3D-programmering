#pragma once

class BufferDescData
{
public:
   D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
   UINT CpuAccess = 0;
   UINT Misc = 0;
   UINT Bind = 0;
   UINT ByteWidth = 0;
   UINT StructureByteStride = 0;
};
