#pragma once

class ConfigBase
{
public:

    // We don't want to copy instances
    ConfigBase(const ConfigBase& other) = delete;
    ConfigBase operator=(const ConfigBase& other) = delete;

    ConfigBase() = default;
};
