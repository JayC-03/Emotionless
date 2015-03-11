#include "core/memmap.h"
#include "common/log.h"
#include "core/hw/dmac.h"

namespace MemoryEE
{
u32 bios[0x100000];

u32 ram[0x800000];
u32 spr[0x1000];

u8 Read8(u32 phys_addr)
{
    u32 res = 0;
    //RAM
    if(phys_addr < 0x02000000 && phys_addr >= 0x00000000) res = ram[(phys_addr & 0x1fffffc) >> 2];
	//Scratchpad RAM
    else if(phys_addr < 0x02004000 && phys_addr >= 0x02000000) res = spr[(phys_addr & 0x3ffc) >> 2];
	//IOP RAM
    else if(phys_addr < 0x1c800000 && phys_addr >= 0x1c000000) res = MemoryIOP::ram[(phys_addr & 0x1ffffc) >> 2];
    //BIOS region
    else if(phys_addr < 0x20000000 && phys_addr >= 0x1fc00000) res = bios[(phys_addr & 0x3ffffc) >> 2];
    else log_print("MemoryEE", "Unrecognized Read8 from physical address " + to_string(phys_addr), log_level::warning);
    //res = bswap32(res);
    res >>= (phys_addr & 3) << 8;
    res &= 0xff;
    return res;
}

u16 Read16(u32 phys_addr)
{
    u32 res = 0;
    //RAM
    if(phys_addr < 0x02000000 && phys_addr >= 0x00000000) res = ram[(phys_addr & 0x1fffffc) >> 2];
	//Scratchpad RAM
    else if(phys_addr < 0x02004000 && phys_addr >= 0x02000000) res = spr[(phys_addr & 0x3ffc) >> 2];
	//IOP RAM
    else if(phys_addr < 0x1c800000 && phys_addr >= 0x1c000000) res = MemoryIOP::ram[(phys_addr & 0x1ffffc) >> 2];
    //BIOS region
    else if(phys_addr < 0x20000000 && phys_addr >= 0x1fc00000) res = bios[(phys_addr & 0x3ffffc) >> 2];
    else log_print("MemoryEE", "Unrecognized Read8 from physical address " + to_string(phys_addr), log_level::warning);
    //res = bswap32(res);
    res >>= (phys_addr & 1) << 16;
    res &= 0xffff;
    return res;
}

u32 Read32(u32 phys_addr)
{
    u32 res = 0;
    //RAM
    if(phys_addr < 0x02000000 && phys_addr >= 0x00000000) res = ram[(phys_addr & 0x1fffffc) >> 2];
	//Scratchpad RAM
    else if(phys_addr < 0x02004000 && phys_addr >= 0x02000000) res = spr[(phys_addr & 0x3ffc) >> 2];
	//IOP RAM
    else if(phys_addr < 0x1c800000 && phys_addr >= 0x1c000000) res = MemoryIOP::ram[(phys_addr & 0x1ffffc) >> 2];
    //BIOS region
    else if(phys_addr < 0x20000000 && phys_addr >= 0x1fc00000) res = bios[(phys_addr & 0xffffc) >> 2];
    else log_print("MemoryEE", "Unrecognized Read32 from physical address " + to_string(phys_addr), log_level::warning);
    //res = bswap32(res);
    return res;
}

u64 Read64(u32 phys_addr)
{
    log_print("MemoryEE", "Read64 from physical address " + to_string(phys_addr), log_level::warning);
    return ((u64)Read32(phys_addr + 4) << 32) | Read32(phys_addr);
}

void Write8(u32 phys_addr, u8 data)
{
    log_print("MemoryEE", "Write8 at physical address " + to_string(phys_addr), log_level::warning);
    u32 temp = Read32(phys_addr);
    temp &= 0xFF << ((phys_addr & 3) << 8);
    temp |= data << ((phys_addr & 3) << 8);
    Write32(phys_addr,temp);
}

void Write32(u32 phys_addr, u32 data)
{
    //RAM
    if(phys_addr >= 0x00000000 && phys_addr < 0x02000000) ram[(phys_addr & 0x1fffffc) >> 2] = data;
	//Scratchpad RAM
    else if(phys_addr >= 0x02000000 && phys_addr < 0x02004000) spr[(phys_addr & 0x3ffc) >> 2] = data;
	//IOP RAM
    else if(phys_addr >= 0x1c000000 && phys_addr < 0x1c800000) MemoryIOP::ram[(phys_addr & 0x1ffffc) >> 2] = data;
    //Extended DMAC area
    if(phys_addr >= 0x1000F500 && phys_addr < 0x1000F600) DMAC::Write32(phys_addr, data);
    log_print("MemoryEE", "Write32 at physical address " + to_string(phys_addr), log_level::warning);
}

void Write64(u32 phys_addr, u64 data)
{
    log_print("MemoryEE", "Write64 at physical address " + to_string(phys_addr), log_level::warning);
    Write32(phys_addr,data&0xffffffff);
    Write32(phys_addr + 4, data >> 32);
}
}

namespace MemoryIOP
{
u32 bios[0x100000];

u32 ram[0x80000];

u8 Read8(u32 phys_addr)
{
    u32 res = 0;
	//RAM
    if(phys_addr < 0x00200000 && phys_addr >= 0x00000000) res = ram[(phys_addr & 0x1ffffc) >> 2];
    //BIOS region
    else if(phys_addr < 0x20000000 && phys_addr >= 0x1fc00000) res = MemoryEE::bios[(phys_addr & 0x3ffffc) >> 2];
    else log_print("MemoryIOP", "Unrecognized Read8 from physical address " + to_string(phys_addr), log_level::warning);
    //res = bswap32(res);
    res >>= (phys_addr & 3) << 8;
    res &= 0xff;
    return res;
}

u32 Read32(u32 phys_addr)
{
    u32 res = 0;
    //RAM
    if(phys_addr < 0x00200000 && phys_addr >= 0x00000000) res = ram[(phys_addr & 0x1ffffc) >> 2];
    //BIOS region
    else if(phys_addr < 0x20000000 && phys_addr >= 0x1fc00000) res = MemoryEE::bios[(phys_addr & 0xffffc) >> 2];
    else log_print("MemoryIOP", "Unrecognized Read32 from physical address " + to_string(phys_addr), log_level::warning);
    //res = bswap32(res);
    return res;
}

void Write8(u32 phys_addr, u8 data)
{
    log_print("MemoryIOP", "Write8 at physical address " + to_string(phys_addr), log_level::warning);
    u32 temp = Read32(phys_addr);
    temp &= 0xFF << ((phys_addr & 3) << 8);
    temp |= data << ((phys_addr & 3) << 8);
    Write32(phys_addr,temp);
}

void Write32(u32 phys_addr, u32 data)
{
    //RAM
    if(phys_addr >= 0x00000000 && phys_addr < 0x00200000) ram[(phys_addr & 0x1ffffc) >> 2] = data;
    log_print("MemoryIOP", "Write32 at physical address " + to_string(phys_addr), log_level::warning);
}
}
