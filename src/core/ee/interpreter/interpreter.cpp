#include "core/ee/interpreter/interpreter.h"
#include "core/ee/interpreter/interpreter_tables.h"
#include "core/ee/ee.h"
#include "common/log.h"

std::function<void(ee_inst)> ee_interpreter::op_table[64];
std::function<void(ee_inst)> ee_interpreter::op_table0[64];
std::function<void(ee_inst)> ee_interpreter::op_table16[32];

void ee_interpreter::init()
{
    PC = 0xbfc00000;
    rCOP0[15] = 0x00002e20; //TODO: Value taken from PCSX2. VERIFY
    EE::interpreter_tables::init_tables();
}

void ee_interpreter::shutdown()
{
}

void ee_interpreter::single_step()
{
    static ee_inst inst_code;

    NPC = PC + sizeof(ee_inst);

    log_print("EE Interpreter", "PC: " + to_string(PC), log_level::verbose);

    inst_code.hex = EE::Read32(PC);

    log_print("EE Interpreter", "Instruction: " + to_string(inst_code.hex), log_level::verbose);

    log_print("EE Interpreter", "Instruction Opcode: " + to_string(inst_code.opcd), log_level::verbose);

    if(EE::ee_state.branch)
    {
        EE::ee_state.branch = 0;
        if(EE::ee_state.branch_likely)
        {
            EE::ee_state.branch_likely = 0;
            if(EE::ee_state.condition) PC += EE::ee_state.branch_offset;
            else goto skip_opcode;
        }
        else if(EE::ee_state.condition) PC += EE::ee_state.branch_offset;
    }
    
    else if(EE::ee_state.jump)
    {
        EE::ee_state.jump = 0;
        PC = EE::ee_state.jump_target;
    }

    op_table[inst_code.opcd](inst_code);

skip_opcode:

    PC += 4;
}

void ee_interpreter::unknown(ee_inst inst)
{
    log_print("EE Interpreter", "Unknown instruction " + to_string(inst.hex) + " at address " + to_string(PC), log_level::error);
}

void ee_interpreter::run_table0(ee_inst inst) {
    op_table0[inst.funct](inst);
}
void ee_interpreter::run_table16(ee_inst inst) {
    op_table16[inst.RS](inst);
}