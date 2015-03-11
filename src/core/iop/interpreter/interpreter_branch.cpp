#include "core/iop/interpreter/interpreter.h"
#include "core/iop/iop.h"

void iop_interpreter::blezl(iop_inst inst)
{
    IOP::iop_state.jump = 1;
    IOP::iop_state.condition = (s32)rGPR[inst.RS] <= 0;
    IOP::iop_state.jump_likely = 1;
    IOP::iop_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void iop_interpreter::bgez(iop_inst inst)
{
    IOP::iop_state.jump = 1;
    IOP::iop_state.condition = (s32)rGPR[inst.RS] >= 0;
    IOP::iop_state.jump_likely = 0;
    IOP::iop_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void iop_interpreter::blez(iop_inst inst)
{
    IOP::iop_state.jump = 1;
    IOP::iop_state.condition = (s32)rGPR[inst.RS] <= 0;
    IOP::iop_state.jump_likely = 0;
    IOP::iop_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void iop_interpreter::bne(iop_inst inst)
{
    IOP::iop_state.jump = 1;
    IOP::iop_state.condition = rGPR[inst.RS] != rGPR[inst.RT];
    IOP::iop_state.jump_likely = 0;
    IOP::iop_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void iop_interpreter::beq(iop_inst inst)
{
    IOP::iop_state.jump = 1;
    IOP::iop_state.condition = rGPR[inst.RS] == rGPR[inst.RT];
    IOP::iop_state.jump_likely = 0;
    IOP::iop_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void iop_interpreter::bnel(iop_inst inst)
{
    IOP::iop_state.jump = 1;
    IOP::iop_state.condition = rGPR[inst.RS] != rGPR[inst.RT];
    IOP::iop_state.jump_likely = 1;
    IOP::iop_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void iop_interpreter::beql(iop_inst inst)
{
    IOP::iop_state.jump = 1;
    IOP::iop_state.condition = rGPR[inst.RS] != rGPR[inst.RT];
    IOP::iop_state.jump_likely = 1;
    IOP::iop_state.jump_target = (PC + 4) + (((s32)inst.SIMM_16) << 2);
}

void iop_interpreter::jr(iop_inst inst)
{
    IOP::iop_state.jump = 1;
	IOP::iop_state.condition = 1;
    IOP::iop_state.jump_target = rGPR[inst.RS];
}

void iop_interpreter::jalr(iop_inst inst)
{
    IOP::iop_state.jump = 1;
	IOP::iop_state.condition = 1;
    IOP::iop_state.jump_target = rGPR[inst.RS];
    rGPR[inst.RD] = PC + 8;
}

void iop_interpreter::jal(iop_inst inst)
{
    IOP::iop_state.jump = 1;
	IOP::iop_state.condition = 1;
    IOP::iop_state.jump_target = ((PC + 4) & 0xF0000000) | ((inst.instr_index) << 2);
    rGPR[31] = PC + 8;
}
