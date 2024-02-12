#pragma once

class IFileParser
{
public:
    IFileParser() = default;
    virtual ~IFileParser() = default;
    
protected:
    virtual void ReadFile() = 0;
};