#include "Controller.hpp"

#include "D3D11Helper.hpp"

Controller::Controller(ID3D11Device *device, ID3D11DeviceContext *immediateContext, D3D11_VIEWPORT viewport)
{
    this->_device = device;
    this->_immediateContext = immediateContext;
    this->_viewport = viewport;
}


Controller& Controller::operator=(Controller&& other) noexcept
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

Controller::~Controller()
{
    this->_device->Release();
    this->_immediateContext->Release();
}

ID3D11DeviceContext *Controller::GetContext()
{
    return this->_immediateContext;
}

ID3D11Device *Controller::GetDevice() const
{
    return this->_device;
}

D3D11_VIEWPORT Controller::GetViewPort()
{
    return this->_viewport;
}





