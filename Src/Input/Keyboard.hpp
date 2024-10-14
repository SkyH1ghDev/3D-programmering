#pragma once

#include "Key.hpp"
#include <vector>
#include <memory>

class Keyboard
{
public:
    Keyboard() = default;
    Keyboard(const Keyboard& other);
    Keyboard& operator=(const Keyboard& other);
    Keyboard(const std::vector<std::shared_ptr<Key>>& keys);

public:
    std::vector<std::shared_ptr<Key>> GetPressedKeys();
    
private:
    std::vector<std::shared_ptr<Key>> _keys;
};
