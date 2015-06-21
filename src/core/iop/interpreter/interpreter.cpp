#include "core/iop/interpreter/interpreter.h"
#include "core/iop/interpreter/interpreter_tables.h"
#include "core/iop/iop.h"

#include "core/main.h"

#include "common/log.h"

std::function<void(iop_inst)> iop_interpreter::op_table[64];
std::function<void(iop_inst)> iop_interpreter::op_table0[64];
std::function<void(iop_inst)> iop_interpreter::op_table1[32];
std::function<void(iop_inst)> iop_interpreter::op_table16[32];
std::function<void(iop_inst)> iop_interpreter::op_table16_16[64];

void iop_interpreter::init()
{
    PC = 0xbfc00000;
    rCOP0[IOP::COP0_regs::Cause] = 0;
    rCOP0[15] = 0x0000001f; //TODO: Value taken from hpsx64. VERIFY
    IOP::iop_state.jump = 0;
    IOP::iop_state.branch = 0;
    IOP::interpreter_tables::init_tables();
}

void iop_interpreter::shutdown()
{
}

void iop_interpreter::single_step()
{
    static iop_inst inst_code;

    log_print("IOP Interpreter", log_level::verbose, "PC: %08x", PC);

    for(int i = 0; i < 32; i++)
    {
        log_print("IOP Interpreter", log_level::verbose, "R%02x: %08x", i, rGPR[i]);
    }

    inst_code.hex = IOP::Read32(PC);

    log_print("IOP Interpreter", log_level::verbose, "Instruction: %08x", inst_code.hex);

    log_print("IOP Interpreter", log_level::verbose, "Instruction RS: %02x", inst_code.RS);

    log_print("IOP Interpreter", log_level::verbose, "Instruction RT: %02x", inst_code.RT);

    log_print("IOP Interpreter", log_level::verbose, "Instruction RD: %02x", inst_code.RD);

    log_print("IOP Interpreter", log_level::verbose, "Instruction Opcode: %02x", inst_code.opcd);

    if(IOP::iop_state.jump)
    {
        IOP::iop_state.jump = 0;
        if(IOP::iop_state.jump_likely)
        {
            if(IOP::iop_state.condition) IOP::iop_state.branch = 1;
            else PC += 4;
        }
        else
        {
            if(IOP::iop_state.condition) IOP::iop_state.branch = 1;
        }
    }

    op_table[inst_code.opcd](inst_code);

    if(IOP::iop_state.branch) PC = IOP::iop_state.jump_target;
    else PC += 4;

    IOP::iop_state.branch = 0;
}

void iop_interpreter::unknown(iop_inst inst)
{
    log_print("IOP Interpreter", log_level::error, "Unknown instruction %08x at address %08x", inst.hex, PC);
}

void iop_interpreter::exception()
{
    log_print("IOP Interpreter", log_level::debug, "Exception at address %08x", PC);
    rCOP0[IOP::COP0_regs::EPC] = PC;

    //TODO: Fully implement this!

    //Checking BEV
    if(rCOP0[IOP::COP0_regs::Status] & 0x400000) PC = 0xbfc0037c;
    else PC = 0x8000017c;

    IOP::iop_state.branch = 0;
}

void iop_interpreter::run_table0(iop_inst inst)
{
    op_table0[inst.funct](inst);
}

void iop_interpreter::run_table1(iop_inst inst)
{
    op_table1[inst.RT](inst);
}

void iop_interpreter::run_table16(iop_inst inst)
{
    op_table16[inst.RS](inst);
}

void iop_interpreter::run_table16_16(iop_inst inst)
{
    op_table16_16[inst.funct](inst);
}
