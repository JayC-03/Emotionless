#pragma once

class iop_core_base
{
public:
    virtual ~iop_core_base() {};

    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual void clear_cache() {}
    virtual void run() {}
    virtual void single_step() = 0;
};
