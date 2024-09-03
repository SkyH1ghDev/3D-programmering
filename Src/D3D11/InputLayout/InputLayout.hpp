#pragma once

#include <d3d11_4.h>

class InputLayout
{
public:
	InputLayout() = delete;
	~InputLayout();

	InputLayout(ID3D11InputLayout* inputLayout);
	
	InputLayout(const InputLayout& other) = delete;
	InputLayout& operator=(const InputLayout& other) = delete;
	InputLayout(InputLayout&& other) = delete;
	InputLayout& operator=(InputLayout&& other) = delete;

	//void AddInputElement(const std::string& semanticName, DXGI_FORMAT format); // Unused
	//void FinalizeInputLayout(ID3D11Device* device, const void* vsDataPtr, size_t vsDataSize); // Unused
	
	ID3D11InputLayout* GetInputLayout() const;
	
private:
	//std::vector<D3D11_INPUT_ELEMENT_DESC> _inputDesc;
	//std::vector<std::pair<const std::string&, DXGI_FORMAT>> _elementList;
	ID3D11InputLayout* _inputLayout = nullptr;
};