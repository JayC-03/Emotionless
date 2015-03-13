#pragma once

#undef PC
#undef NPC
#undef rGPR
#undef rCOP0

#include "common/common_types.h"
#include "common/log.h"
#include "core/iop/interpreter/interpreter.h"
#include "core/memmap.h"

namespace IOP
{

extern iop_interpreter* interpreter;

struct iop_state_t
{
    u32 gpr[32];

    u32 cop0r[32];

    u32 pc;
    u32 npc;
    u32 condition;
    u32 code;
    u32 jump;
    u32 jump_likely;
    u32 jump_target;

    u32 branch;
};

enum COP0_regs
{
    BadVAddr = 8,
    Status = 12,
    Cause = 13,
    EPC = 14,
};

extern iop_state_t iop_state;

#define PC IOP::iop_state.pc
#define NPC IOP::iop_state.npc
#define rGPR IOP::iop_state.gpr
#define rCOP0 IOP::iop_state.cop0r

u32 TranslateAddress(u32 virt_addr);

u8 Read8(u32 virt_addr);
u32 Read32(u32 virt_addr);

void Write8(u32 virt_addr, u8 data);
void Write32(u32 virt_addr, u32 data);

}
