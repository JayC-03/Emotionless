#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::mfc0(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = (s64)(s32)rCOP0[inst.RD];
}

void ee_interpreter::mtc0(ee_inst inst)
{
    rCOP0[inst.RD] = rGPR[inst.RT].ul[0];
}

void ee_interpreter::tlbwi(ee_inst inst)
{
    //TODO: Implement TLB
}