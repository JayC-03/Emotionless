#include "core/iop/interpreter/interpreter.h"
#include "core/iop/iop.h"

void iop_interpreter::sll(iop_inst inst)
{
    rGPR[inst.RD] = (s32)(rGPR[inst.RT] << inst.SA);
}

void iop_interpreter::srl(iop_inst inst)
{
    rGPR[inst.RD] = (s32)(rGPR[inst.RT] >> inst.SA);
}

void iop_interpreter::sra(iop_inst inst)
{
    rGPR[inst.RD] = (s32)((s32)rGPR[inst.RT] >> inst.SA);
}

void iop_interpreter::slt(iop_inst inst)
{
    rGPR[inst.RD] = 0;
    if(rGPR[inst.RS] < rGPR[inst.RT]) rGPR[inst.RD] = 1;
    else rGPR[inst.RD] = 0;
}

void iop_interpreter::sltu(iop_inst inst)
{
    rGPR[inst.RD] = 0;
    if(rGPR[inst.RS] < rGPR[inst.RT]) rGPR[inst.RD] = 1;
    else rGPR[inst.RD] = 0;
}

void iop_interpreter::slti(iop_inst inst)
{
    rGPR[inst.RT] = 0;
    if(rGPR[inst.RS] < (s64)inst.SIMM_16) rGPR[inst.RT] = 1;
    else rGPR[inst.RT] = 0;
}

void iop_interpreter::sltiu(iop_inst inst)
{
    rGPR[inst.RT] = 0;
    if(rGPR[inst.RS] < (u64)(s64)inst.SIMM_16) rGPR[inst.RT] = 1;
    else rGPR[inst.RT] = 0;
}

void iop_interpreter::and_iop(iop_inst inst)
{
    rGPR[inst.RD] = rGPR[inst.RS] & rGPR[inst.RT];
}

void iop_interpreter::andi(iop_inst inst)
{
    rGPR[inst.RT] = rGPR[inst.RS] & inst.UIMM_16;
}

void iop_interpreter::ori(iop_inst inst)
{
    rGPR[inst.RT] = rGPR[inst.RS] | inst.UIMM_16;
}

void iop_interpreter::or_iop(iop_inst inst)
{
    rGPR[inst.RD] = rGPR[inst.RS] | rGPR[inst.RT];
}

void iop_interpreter::addi(iop_inst inst)
{
    rGPR[inst.RT] = (s64)(rGPR[inst.RS] + (s32)inst.SIMM_16);
}

void iop_interpreter::addu(iop_inst inst)
{
    rGPR[inst.RD] = (s64)(s32)(rGPR[inst.RS] + rGPR[inst.RT]);
}

void iop_interpreter::addiu(iop_inst inst)
{
    rGPR[inst.RT] = (s64)(rGPR[inst.RS] + (s32)inst.SIMM_16);
}

void iop_interpreter::sync(iop_inst inst)
{
    //TODO: This doesn't need to be implemented yet.
}
