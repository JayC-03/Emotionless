#include "core/memmap.h"
#include "common/log.h"
#include "core/hw/dmac.h"

namespace Memory
{
u32 bios[0x100000];

u32 Read32(u32 phys_addr)
{
    u32 res = 0;
    //BIOS region
    if(phys_addr < 0x20000000 && phys_addr >= 0x1fc00000) res = bios[(phys_addr & 0xffffc) >> 2];
    else log_print("Memory", "Unrecognized Read32 from physical address " + to_string(phys_addr), log_level::warning);
    //res = bswap32(res);
    return res;
}

void Write32(u32 phys_addr, u32 data)
{
    //Extended DMAC area
    if(phys_addr >= 0x1000F500 && phys_addr < 0x1000F600) DMAC::Write32(phys_addr, data);
    log_print("Memory", "Write32 at physical address " + to_string(phys_addr), log_level::warning);
}

void Write64(u32 phys_addr, u64 data)
{
    log_print("Memory", "Write64 at physical address " + to_string(phys_addr), log_level::warning);
}
}