#include "core/iop/interpreter/interpreter.h"
#include "core/iop/iop.h"

void iop_interpreter::mfc0(iop_inst inst)
{
    rGPR[inst.RT] = (s64)(s32)rCOP0[inst.RD];
}

void iop_interpreter::mtc0(iop_inst inst)
{
    rCOP0[inst.RD] = rGPR[inst.RT];
}

void iop_interpreter::tlbwi(iop_inst inst)
{
    //TODO: Implement TLB
}

void iop_interpreter::break_iop(iop_inst inst)
{
    rCOP0[IOP::COP0_regs::Cause] = 0x24;
    rCOP0[IOP::COP0_regs::Status] |= 0x2;
    iop_interpreter::exception();
}
