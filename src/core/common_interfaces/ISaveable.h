#pragma once

class ISaveable
{
public:
    virtual ~ISaveable() = default;

    virtual bool save() = 0;
    virtual bool load() = 0;
};