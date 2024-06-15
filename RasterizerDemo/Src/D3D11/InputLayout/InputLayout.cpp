#include "InputLayout.hpp"

void InputLayout::AddInputElement(const std::string &semanticName, DXGI_FORMAT format)
{
   this->_elementList.push_back(std::make_pair(semanticName, format));
}

void InputLayout::FinalizeInputLayout(ID3D11Device *device, const void *vsDataPtr, size_t vsDataSize)
{
   bool notFirstElement = false;
   
   for (auto element : this->_elementList)
   {
      this->_inputDesc.push_back({element.first.c_str(), 0, element.second, 0, D3D11_APPEND_ALIGNED_ELEMENT * notFirstElement, D3D11_INPUT_PER_VERTEX_DATA, 0});
      notFirstElement = true;
   }
}

ID3D11InputLayout* InputLayout::GetInputLayout() const
{
   return this->_inputLayout;
}

InputLayout::~InputLayout()
{
   this->_inputLayout->Release();
}

