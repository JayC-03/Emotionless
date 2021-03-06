#pragma once

#undef PC
#undef NPC
#undef rGPR
#undef rFPR
#undef rLO
#undef rHI
#undef rCOP0

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

    u32 cop1r[32];

    u32 pc;
    u32 npc;
    u32 condition;
    u32 code;
    u32 jump;
    u32 jump_likely;
    u32 jump_target;

    u32 downcount;

    u32 branch;
};

enum COP0_regs
{
    BadVAddr = 8,
    Count = 9,
    Status = 12,
    Cause = 13,
    EPC = 14,
};

extern ee_state_t ee_state;

#define PC EE::ee_state.pc
#define NPC EE::ee_state.npc
#define rGPR EE::ee_state.gpr
#define rFPR EE::ee_state.cop1r
#define rLO EE::ee_state.lo
#define rHI EE::ee_state.hi
#define rCOP0 EE::ee_state.cop0r

u32 TranslateAddress(u32 virt_addr);

u8 Read8(u32 virt_addr);
u16 Read16(u32 virt_addr);
u32 Read32(u32 virt_addr);
u64 Read64(u32 virt_addr);

void Write8(u32 virt_addr, u8 data);
void Write16(u32 virt_addr, u16 data);
void Write32(u32 virt_addr, u32 data);
void Write64(u32 virt_addr, u64 data);
void Write128(u32 virt_addr, ee_reg data);

}
