#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::sll(ee_inst inst)
{
    rGPR[inst.RD].ud[0] = (s64)(s32)(rGPR[inst.RT].ul[0] << inst.SA);
}

void ee_interpreter::slti(ee_inst inst)
{
    rGPR[inst.RT].ud[1] = 0;
    if(rGPR[inst.RT].ud[0] < (s64)inst.SIMM_16) rGPR[inst.RT].ud[0] = 1;
    else rGPR[inst.RT].ud[0] = 0;
}

void ee_interpreter::ori(ee_inst inst)
{
    rGPR[inst.RT].ud[0] |= inst.UIMM_16;
}