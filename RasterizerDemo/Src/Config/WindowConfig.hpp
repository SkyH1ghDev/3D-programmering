#pragma once

#include <windows.h>
#include "IConfigBase.hpp"

class WindowConfig : public IConfigBase
{
public:
   int GetHeight() const { return this->_height; }
   int GetWidth() const { return this->_width; }
   LPCWSTR GetWindowName() const { return this->_windowName; }
   
private:
   int _height = 720;
   int _width = 1280;
   LPCWSTR _windowName = L"3D-Renderer - Christoffer Bohman";
};
