#include "D3D11Controller.hpp"

#include "D3D11Helper.hpp"

D3D11Controller::D3D11Controller(ID3D11Device *device, ID3D11DeviceContext *immediateContext, D3D11_VIEWPORT viewport)
{
    this->_device = device;
    this->_immediateContext = immediateContext;
    this->_viewport = viewport;
}


D3D11Controller& D3D11Controller::operator=(D3D11Controller&& other) noexcept
{
    if (&other == this)
    {
        return *this;
    }
    
    this->_viewport = other._viewport;

    this->_device = other._device; this->_device->AddRef();
    this->_immediateContext = other._immediateContext; this->_immediateContext->AddRef();
    
    return *this;
}

D3D11Controller::~D3D11Controller()
{
    this->_device->Release();
    this->_immediateContext->Release();
}

ID3D11DeviceContext *D3D11Controller::GetContext()
{
    return this->_immediateContext;
}

ID3D11Device *D3D11Controller::GetDevice()
{
    return this->_device;
}

D3D11_VIEWPORT D3D11Controller::GetViewPort()
{
    return this->_viewport;
}





