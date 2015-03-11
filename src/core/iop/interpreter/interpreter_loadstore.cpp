#include "core/iop/interpreter/interpreter.h"
#include "core/iop/iop.h"

void iop_interpreter::lui(iop_inst inst)
{
    rGPR[inst.RT] = (u64)(s64)(s32)(inst.UIMM_16 << 16);
}

void iop_interpreter::lb(iop_inst inst)
{
    rGPR[inst.RT] = (s64)(s8)IOP::Read8(rGPR[inst.RS] + (s32)inst.SIMM_16);
}

void iop_interpreter::lbu(iop_inst inst)
{
    rGPR[inst.RT] = IOP::Read8(rGPR[inst.RS] + (s32)inst.SIMM_16);
}

void iop_interpreter::lw(iop_inst inst)
{
    rGPR[inst.RT] = (s64)(s32)IOP::Read32(rGPR[inst.RS] + (s32)inst.SIMM_16);
}

void iop_interpreter::sb(iop_inst inst)
{
    IOP::Write8(rGPR[inst.RS] + (s32)inst.SIMM_16, rGPR[inst.RT]);
}

void iop_interpreter::sw(iop_inst inst)
{
    IOP::Write32(rGPR[inst.RS] + (s32)inst.SIMM_16, rGPR[inst.RT]);
}
