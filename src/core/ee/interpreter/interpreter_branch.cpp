#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::blezl(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = rGPR[inst.RS].sd[0] <= 0;
    EE::ee_state.jump_likely = 1;
    EE::ee_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void ee_interpreter::bltz(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = rGPR[inst.RS].sd[0] < 0;
    EE::ee_state.jump_likely = 0;
    EE::ee_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void ee_interpreter::bgez(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = rGPR[inst.RS].sd[0] >= 0;
    EE::ee_state.jump_likely = 0;
    EE::ee_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void ee_interpreter::blez(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = rGPR[inst.RS].sd[0] <= 0;
    EE::ee_state.jump_likely = 0;
    EE::ee_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void ee_interpreter::bgtz(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = rGPR[inst.RS].sd[0] > 0;
    EE::ee_state.jump_likely = 0;
    EE::ee_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void ee_interpreter::bne(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = rGPR[inst.RS].ud[0] != rGPR[inst.RT].ud[0];
    EE::ee_state.jump_likely = 0;
    EE::ee_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void ee_interpreter::beq(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = rGPR[inst.RS].ud[0] == rGPR[inst.RT].ud[0];
    EE::ee_state.jump_likely = 0;
    EE::ee_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void ee_interpreter::bnel(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = rGPR[inst.RS].ud[0] != rGPR[inst.RT].ud[0];
    EE::ee_state.jump_likely = 1;
    EE::ee_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void ee_interpreter::beql(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = rGPR[inst.RS].ud[0] == rGPR[inst.RT].ud[0];
    EE::ee_state.jump_likely = 1;
    EE::ee_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void ee_interpreter::jr(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = 1;
    EE::ee_state.jump_target = rGPR[inst.RS].ul[0];
}

void ee_interpreter::j(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = 1;
    EE::ee_state.jump_target = ((PC + 4) & 0xF0000000) | ((inst.instr_index) << 2);
}

void ee_interpreter::jalr(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = 1;
    EE::ee_state.jump_target = rGPR[inst.RS].ul[0];
    rGPR[inst.RD].ud[0] = PC + 8;
}

void ee_interpreter::jal(ee_inst inst)
{
    EE::ee_state.jump = 1;
    EE::ee_state.condition = 1;
    EE::ee_state.jump_target = ((PC + 4) & 0xF0000000) | ((inst.instr_index) << 2);
    rGPR[31].ud[0] = PC + 8;
}
