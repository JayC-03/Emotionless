#pragma once

#include "common/common_types.h"
#include "common/log.h"
#include "core/ee/interpreter/interpreter.h"
#include "core/memmap.h"

namespace EE
{

extern ee_interpreter* interpreter;

union ee_reg
{
    u64 ud[2];
    s64 sd[2];
    u32 ul[4];
    s32 sl[4];
    u16 us[8];
    s16 ss[8];
    u8 uc[16];
    s8 sc[16];
};

struct ee_state_t
{
    ee_reg gpr[32];

    ee_reg hi,lo;

    u32 cop0r[32];

    u32 pc;
    u32 npc;
    u32 condition;
    u32 code;
    u32 branch;
    u32 branch_likely;
    s32 branch_offset;
    u32 jump;
    u32 jump_target;
};

extern ee_state_t ee_state;

#define PC EE::ee_state.pc
#define NPC EE::ee_state.npc
#define rGPR EE::ee_state.gpr
#define rCOP0 EE::ee_state.cop0r

u32 TranslateAddress(u32 virt_addr);

u32 Read32(u32 virt_addr);

void Write32(u32 virt_addr, u32 data);
void Write64(u32 virt_addr, u64 data);

}
