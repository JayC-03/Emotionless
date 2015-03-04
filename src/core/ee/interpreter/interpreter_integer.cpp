#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::sll(ee_inst inst)
{
  rGPR[inst.RD].ud[0] = (s64)(s32)(rGPR[inst.RT].ul[0] << inst.SA);
}