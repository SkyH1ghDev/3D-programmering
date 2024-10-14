#define KEY_PRESSED 0x8000

#include "Keyboard.hpp"
#include <windows.h>
#include <iostream>

Keyboard::Keyboard(const Keyboard& other)
{
    this->_keys = other._keys;
}

Keyboard& Keyboard::operator=(const Keyboard& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->_keys = other._keys;
    return *this;
}

Keyboard::Keyboard(const std::vector<std::shared_ptr<Key>>& keys)
{
    this->_keys = keys;
}

std::vector<std::shared_ptr<Key>> Keyboard::GetPressedKeys()
{
    std::vector<std::shared_ptr<Key>> pressedKeys;

    for (std::shared_ptr<Key>& key : this->_keys)
    {

        key->SetPressedState(GetAsyncKeyState(key->GetVirtualKey()) & KEY_PRESSED);
        
        if (key->IsPressed() || key->IsHeldDown())
        {
            pressedKeys.push_back(key);
        }
    }

    return pressedKeys;
}

