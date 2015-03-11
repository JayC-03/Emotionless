#include "core/iop/interpreter/interpreter_tables.h"
#include "core/iop/interpreter/interpreter.h"
#include "core/iop/iop.h"
#include "core/iop/iop_inst.h"

namespace IOP
{
namespace interpreter_tables
{

struct iop_op_template
{
    u32 opcode;
    std::function<void(iop_inst)> inst;
};

static iop_op_template primary_table[] =
{
    {0,  iop_interpreter::run_table0},
    {1,  iop_interpreter::run_table1},
    {3,  iop_interpreter::jal},
    {4,  iop_interpreter::beq},
    {5,  iop_interpreter::bne},
	{6,  iop_interpreter::blez},
	{8,  iop_interpreter::addi},
    {9,  iop_interpreter::addiu},
    {10, iop_interpreter::slti},
    {11, iop_interpreter::sltiu},
	{12, iop_interpreter::andi},
    {13, iop_interpreter::ori},
    {15, iop_interpreter::lui},
    {16, iop_interpreter::run_table16},
    {20, iop_interpreter::beql},
    {21, iop_interpreter::bnel},
    {22, iop_interpreter::blezl},
    {32, iop_interpreter::lb},
    {35, iop_interpreter::lw},
	{36, iop_interpreter::lbu},
    {40, iop_interpreter::sb},
    {43, iop_interpreter::sw},
};

static iop_op_template table0[] =
{
    {0,  iop_interpreter::sll},
    {2,  iop_interpreter::srl},
	{3,  iop_interpreter::sra},
    {8,  iop_interpreter::jr},
    {9,  iop_interpreter::jalr},
	{13, iop_interpreter::break_iop},
    {15, iop_interpreter::sync},
	{33, iop_interpreter::addu},
    {36, iop_interpreter::and_iop},
    {37, iop_interpreter::or_iop},
    {42, iop_interpreter::slt},
    {43, iop_interpreter::sltu},
};

static iop_op_template table1[] =
{
	{1,  iop_interpreter::bgez},
};

static iop_op_template table16[] =
{
    {0,  iop_interpreter::mfc0},
    {4,  iop_interpreter::mtc0},
    {16, iop_interpreter::run_table16_16},
};

static iop_op_template table16_16[] =
{
    {2, iop_interpreter::tlbwi},
};

void init_tables()
{
    for(int i = 0; i < 64; i++)
    {
        iop_interpreter::op_table[i] = iop_interpreter::unknown;
    }

    for(int i = 0; i < 64; i++)
    {
        iop_interpreter::op_table0[i] = iop_interpreter::unknown;
    }

    for(int i = 0; i < 32; i++)
    {
        iop_interpreter::op_table1[i] = iop_interpreter::unknown;
    }

    for(int i = 0; i < 32; i++)
    {
        iop_interpreter::op_table16[i] = iop_interpreter::unknown;
    }
    
    for(int i = 0; i < 64; i++)
    {
        iop_interpreter::op_table16_16[i] = iop_interpreter::unknown;
    }

    for(auto& tbl_op : primary_table)
    {
        iop_interpreter::op_table[tbl_op.opcode] = tbl_op.inst;
    }

    for(auto& tbl_op : table0)
    {
        iop_interpreter::op_table0[tbl_op.opcode] = tbl_op.inst;
    }

    for(auto& tbl_op : table1)
    {
        iop_interpreter::op_table1[tbl_op.opcode] = tbl_op.inst;
    }

    for(auto& tbl_op : table16)
    {
        iop_interpreter::op_table16[tbl_op.opcode] = tbl_op.inst;
    }
    
    for(auto& tbl_op : table16_16)
    {
        iop_interpreter::op_table16_16[tbl_op.opcode] = tbl_op.inst;
    }
}
}
}
