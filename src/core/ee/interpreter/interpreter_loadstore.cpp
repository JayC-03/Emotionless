#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::lui(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = (u64)(s64)(s32)(inst.UIMM_16 << 16);
}

void ee_interpreter::sw(ee_inst inst)
{
    EE::Write32(rGPR[inst.RS].ul[0] + (s32)inst.SIMM_16, rGPR[inst.RT].ul[0]);
}

void ee_interpreter::sd(ee_inst inst)
{
    EE::Write64(rGPR[inst.RS].ul[0] + (s32)inst.SIMM_16, rGPR[inst.RT].ud[0]);
}