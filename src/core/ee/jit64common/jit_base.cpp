// Copyright 2008 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "core/ee/jit_common/jit_base.h"

JitBase *jit;

void Jit(u32 em_address)
{
    jit->Jit(em_address);
}

u32 Helper_Mask(u8 mb, u8 me)
{
    u32 mask = ((u32)-1 >> mb) ^ (me >= 31 ? 0 : (u32)-1 >> (me + 1));
    return mb > me ? ~mask : mask;
}
