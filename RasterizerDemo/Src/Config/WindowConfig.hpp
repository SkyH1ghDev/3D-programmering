#pragma once

#include "IConfigBase.hpp"

class WindowConfig : public IConfigBase
{
public:
   int GetHeight() const { return this->_height; }
   int GetWidth() const { return this->_width; }
   
private:
   int _height = 720;
   int _width = 1280;
};
