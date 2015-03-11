#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::mfc0(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = rCOP0[inst.RD];
}

void ee_interpreter::mtc0(ee_inst inst)
{
    rCOP0[inst.RD] = rGPR[inst.RT].ul[0];
}

void ee_interpreter::tlbwi(ee_inst inst)
{
    //TODO: Implement TLB
}

void ee_interpreter::break_ee(ee_inst inst)
{
    rCOP0[EE::COP0_regs::Cause] = 0x24;
	rCOP0[EE::COP0_regs::Status] |= 0x2;
	ee_interpreter::exception();
}

void ee_interpreter::syscall(ee_inst inst)
{
    rCOP0[EE::COP0_regs::Cause] = 0x20;
	rCOP0[EE::COP0_regs::Status] |= 0x2;
	ee_interpreter::exception();
}
