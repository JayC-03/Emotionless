#pragma once

#include "common/common.h"

namespace Memory
{
extern u32 ram[0x800000]; //32MB
extern u32 bios[0x100000]; //4MB

u8 Read8(u32 phys_addr);
u32 Read32(u32 phys_addr);
u64 Read64(u32 phys_addr);
void Write32(u32 phys_addr, u32 data);
void Write64(u32 phys_addr, u64 data);
}