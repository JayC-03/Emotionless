#include "core/ee/interpreter/interpreter.h"
#include "core/ee/interpreter/interpreter_tables.h"
#include "core/ee/ee.h"

#include "core/main.h"

#include "common/log.h"

interpreter_func_type ee_interpreter::op_table[64];
interpreter_func_type ee_interpreter::op_table0[64];
interpreter_func_type ee_interpreter::op_table1[32];
interpreter_func_type ee_interpreter::op_table16[32];
interpreter_func_type ee_interpreter::op_table16_16[64];
interpreter_func_type ee_interpreter::op_table28[64];
interpreter_func_type ee_interpreter::op_table28_40[32];

void ee_interpreter::init()
{
    PC = 0xbfc00000;
    rCOP0[EE::COP0_regs::Cause] = 0;
    rCOP0[EE::COP0_regs::Status] = 0x000000;
    rCOP0[15] = 0x00002e20; //TODO: Value taken from PCSX2. VERIFY
    EE::ee_state.jump = 0;
    EE::ee_state.branch = 0;
    EE::interpreter_tables::init_tables();
}

void ee_interpreter::shutdown()
{
}

void ee_interpreter::single_step()
{
    static ee_inst inst_code;

    rGPR[0].ud[0] = 0;
    rGPR[0].ud[1] = 0;

    //TODO: Actually simulate timings
    //HACK
    rCOP0[EE::COP0_regs::Count]++;

    if(EE::ee_state.jump)
    {
        EE::ee_state.jump = 0;
        if(EE::ee_state.jump_likely)
        {
            if(EE::ee_state.condition) EE::ee_state.branch = 1;
            else PC += 4;
        }
        else
        {
            if(EE::ee_state.condition) EE::ee_state.branch = 1;
        }
    }

    log_print("EE Interpreter", log_level::debug, "PC: %08x", PC);

    for(int i = 0; i < 32; i++)
    {
        log_print("EE Interpreter", log_level::verbose, "R%02x: %08x %08x %08x %08x", i, rGPR[i].ul[3], rGPR[i].ul[2], rGPR[i].ul[1], rGPR[i].ul[0]);
    }

    inst_code.hex = EE::Read32(PC);

    log_print("EE Interpreter", log_level::debug, "Instruction: %08x", inst_code.hex);

    log_print("EE Interpreter", log_level::verbose, "Instruction RS: %02x", inst_code.RS);

    log_print("EE Interpreter", log_level::verbose, "Instruction RT: %02x", inst_code.RT);

    log_print("EE Interpreter", log_level::verbose, "Instruction RD: %02x", inst_code.RD);

    log_print("EE Interpreter", log_level::debug, "Instruction Opcode: %02x", inst_code.opcd);

    op_table[inst_code.opcd](inst_code);

    if(EE::ee_state.branch) PC = EE::ee_state.jump_target;
    else PC += 4;

    EE::ee_state.branch = 0;
}

void ee_interpreter::run()
{
	end_block = false;
	int cycles = 0;
	while(!end_block)
	{
		single_step();
		cycles++;
	}
}

void ee_interpreter::unknown(ee_inst inst)
{
    log_print("EE Interpreter", log_level::error, "Unknown instruction %08x at address %08x", inst.hex, PC);
}

void ee_interpreter::exception()
{
    log_print("EE Interpreter", log_level::debug, "Exception at address %08x", PC);
    rCOP0[EE::COP0_regs::EPC] = PC;

    //TODO: Fully implement this!

    //Checking BEV
    if(rCOP0[EE::COP0_regs::Status] & 0x400000) PC = 0xbfc0037c;
    else PC = 0x8000017c;

    EE::ee_state.branch = 0;
}

void ee_interpreter::run_table0(ee_inst inst)
{
    op_table0[inst.funct](inst);
}

void ee_interpreter::run_table1(ee_inst inst)
{
    op_table1[inst.RT](inst);
}

void ee_interpreter::run_table16(ee_inst inst)
{
    op_table16[inst.RS](inst);
}

void ee_interpreter::run_table16_16(ee_inst inst)
{
    op_table16_16[inst.funct](inst);
}

void ee_interpreter::run_table28(ee_inst inst)
{
    op_table28[inst.funct](inst);
}

void ee_interpreter::run_table28_40(ee_inst inst)
{
    op_table28_40[inst.SA](inst);
}
