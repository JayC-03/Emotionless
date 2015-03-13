#pragma once

#include "common/common.h"

namespace GIF
{
struct gif_tag_t
{
    u32 loops : 15;
    u32 eop : 1;
    u32 res0 : 30;
    u32 pre : 1;
    u32 prim : 11;
    u32 cmd : 2;
    u32 nreg : 4;
    u64 regs;
};

void receive_dma(u32 addr);
}
