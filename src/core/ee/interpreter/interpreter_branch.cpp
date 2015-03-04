#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::blezl(ee_inst inst)
{
    EE::ee_state.branch = 1;
    EE::ee_state.condition = rGPR[inst.RS].ul[0] <= 0;
    EE::ee_state.branch_likely = 1;
    EE::ee_state.branch_offset = ((s32)inst.SIMM_16) << 2;
}

void ee_interpreter::bne(ee_inst inst)
{
    EE::ee_state.branch = 1;
    EE::ee_state.condition = rGPR[inst.RS].ud[0] != rGPR[inst.RT].ud[0];
    EE::ee_state.branch_likely = 0;
    EE::ee_state.branch_offset = ((s32)inst.SIMM_16) << 2;
}

void ee_interpreter::jr(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.jump_target = rGPR[inst.RS].ul[0];
}