#pragma once

class gs_core_base
{
public:
    virtual ~gs_core_base() {};

    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual void run() {}
    virtual void draw_frame() = 0;
};
