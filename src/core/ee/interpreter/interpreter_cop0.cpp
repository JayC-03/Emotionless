#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::mfc0(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = (s64)(s32)rCOP0[inst.RD];
}