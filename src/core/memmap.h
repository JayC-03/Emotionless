#pragma once

#include "common/common.h"

namespace Memory
{
extern u32 bios[0x100000]; //4MB

u32 Read32(u32 phys_addr);
}