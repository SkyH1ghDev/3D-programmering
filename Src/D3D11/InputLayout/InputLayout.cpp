#include "InputLayout.hpp"

InputLayout::InputLayout(ID3D11InputLayout* inputLayout)
{
   this->_inputLayout = inputLayout;
}

ID3D11InputLayout* InputLayout::GetInputLayout() const
{
   return this->_inputLayout;
}

InputLayout::~InputLayout()
{
   this->_inputLayout->Release();
}