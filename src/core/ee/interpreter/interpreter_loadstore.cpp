#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::lui(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = (u64)(s64)(s32)(inst.UIMM_16 << 16);
}

void ee_interpreter::lb(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = (s64)(s8)EE::Read8(rGPR[inst.RS].ul[0] + (s32)inst.SIMM_16);
}

void ee_interpreter::lw(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = (s64)(s32)EE::Read32(rGPR[inst.RS].ul[0] + (s32)inst.SIMM_16);
}

void ee_interpreter::ld(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = EE::Read64(rGPR[inst.RS].ul[0] + (s32)inst.SIMM_16);
}

void ee_interpreter::sb(ee_inst inst)
{
    EE::Write8(rGPR[inst.RS].ul[0] + (s32)inst.SIMM_16, rGPR[inst.RT].uc[0]);
}

void ee_interpreter::sw(ee_inst inst)
{
    EE::Write32(rGPR[inst.RS].ul[0] + (s32)inst.SIMM_16, rGPR[inst.RT].ul[0]);
}

void ee_interpreter::sd(ee_inst inst)
{
    EE::Write64(rGPR[inst.RS].ul[0] + (s32)inst.SIMM_16, rGPR[inst.RT].ud[0]);
}

void ee_interpreter::sq(ee_inst inst)
{
    EE::Write128(rGPR[inst.RS].ul[0] + (s32)inst.SIMM_16, rGPR[inst.RT]);
}