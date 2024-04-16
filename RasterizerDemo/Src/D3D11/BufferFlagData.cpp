#include "BufferFlagData.hpp"

BufferFlagData::BufferFlagData()
{
   this->Usage = D3D11_USAGE_DEFAULT;
   this->CpuAccess = static_cast<D3D11_CPU_ACCESS_FLAG>(0);
   this->Misc = static_cast<D3D11_RESOURCE_MISC_FLAG>(0);
}
