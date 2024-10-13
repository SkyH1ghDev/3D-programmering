#pragma once
#include "IConfigBase.hpp"

class InputConfig : IConfigBase
{
public:
    const int& GetQuitKey() const { return this->_quitKey; }

    const int& GetUpKey() const { return this->_upKey; }
    const int& GetDownKey() const { return this->_downKey; }
    const int& GetForwardKey() const { return this->_forwardKey; }
    const int& GetBackKey() const { return this->_backKey; }
    const int& GetLeftKey() const { return this->_leftKey; }
    const int& GetRightKey() const { return this->_rightKey; }
    
    const int& GetOutputModeKey() const { return this->_outputModeKey; }
    const int& GetLockMouseKey() const { return this->_lockMouseKey; }
    
private:
    int _quitKey = VK_ESCAPE;

    int _upKey = VK_SPACE;
    int _downKey = VK_LCONTROL;
    int _forwardKey = 'W';
    int _backKey = 'S';
    int _leftKey = 'A';
    int _rightKey = 'D';
    
    int _outputModeKey = 'N';
    int _lockMouseKey = 'M';
};
