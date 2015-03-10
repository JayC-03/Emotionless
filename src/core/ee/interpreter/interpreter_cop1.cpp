#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::swc1(ee_inst inst)
{
	EE::Write32(rGPR[inst.RS].ul[0] + (s32)inst.SIMM_16, rFPR[inst.RT]);
}
