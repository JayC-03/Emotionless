#include "core/memmap.h"

namespace Memory
{
u32 bios[0x100000];

u32 Read32(u32 phys_addr)
{
    u32 res = 0;
    //BIOS region
    //TODO: not sure if mirrored.
    if(phys_addr < 0x20000000) res = bios[(phys_addr & 0xffffc) >> 2];
    res = bswap32(res);
    return res;
}
}