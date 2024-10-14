#include "Key.hpp"

Key::Key(const int& virtualKey, const InputAction& action)
{
    this->_virtualKey = virtualKey;
    this->_action = action;
}

Key::Key(const int& virtualKey)
{
    this->_virtualKey = virtualKey;
    this->_action = Unbound;
}

bool Key::IsHeldDown()
{
    return this->_isHeldDown;
}

void Key::SetHoldDownState(const bool& val)
{
    this->_isHeldDown = val;
}

bool Key::IsPressed()
{
    return this->_isPressed;
}

void Key::SetPressedState(const bool& val)
{
    this->_isPressed = val;
}

int Key::GetVirtualKey()
{
    return this->_virtualKey;
}

InputAction Key::GetAction()
{
    return this->_action;
}




