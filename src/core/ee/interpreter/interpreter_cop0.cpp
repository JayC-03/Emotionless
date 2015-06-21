#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

#ifdef USE_BIOS_HLE
#include "core/hle/bios_hle.h"
#endif

#include "common/log.h"

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
#ifndef USE_BIOS_HLE
    rCOP0[EE::COP0_regs::Cause] = 0x20;
    rCOP0[EE::COP0_regs::Status] |= 0x2;
    ee_interpreter::exception();
#else
    log_print("HLE", log_level::warning, "Syscall number %02x", rGPR[3].uc[0] & 0x7f);

    HLE::syscalls[rGPR[3].uc[0] & 0x7f](rGPR[3].uc[0] & 0x7f);
#endif
}

void ee_interpreter::ei(ee_inst inst)
{
    rCOP0[EE::COP0_regs::Status] |= 0x00010001;
}

void ee_interpreter::di(ee_inst inst)
{
    rCOP0[EE::COP0_regs::Status] &= 0xfffefffe;
}
