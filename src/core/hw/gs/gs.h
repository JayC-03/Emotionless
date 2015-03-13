#pragma once

#include "common/common.h"

namespace GS
{
struct reg64
{
    u64 actual;
    u64 latched;
    reg64() : latched(0) {}
};
struct gs_state_t
{
    reg64 dispfb1;
    reg64 display1;
    reg64 dispfb2;
    reg64 display2;
};

extern gs_state_t gs_state;

void Write32(u32 addr, u32 data);
}
