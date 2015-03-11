#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::padduw(ee_inst inst)
{
    for(int i = 0; i < 4; i++)
    {
        u64 res = rGPR[inst.RS].ul[i] + rGPR[inst.RT].ul[i];
        rGPR[inst.RD].ul[i] = (res > 0xFFFFFFFF) ? 0xFFFFFFFF : res;
    }
}
