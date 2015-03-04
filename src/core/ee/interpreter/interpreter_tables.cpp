#include "core/ee/interpreter/interpreter_tables.h"
#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"
#include "core/ee/ee_inst.h"

namespace EE
{
namespace interpreter_tables
{

struct ee_op_template
{
    u32 opcode;
    std::function<void(ee_inst)> inst;
};

static ee_op_template primary_table[] =
{
    {0,  ee_interpreter::run_table0},
    {1,  ee_interpreter::run_table1},
    {5,  ee_interpreter::bne},
    {9,  ee_interpreter::addiu},
    {10, ee_interpreter::slti},
    {13, ee_interpreter::ori},
    {15, ee_interpreter::lui},
    {16, ee_interpreter::run_table16},
    {22, ee_interpreter::blezl},
    {43, ee_interpreter::sw},
    {63, ee_interpreter::sd},
};

static ee_op_template table0[] =
{
    {0,  ee_interpreter::sll},
    {8,  ee_interpreter::jr},
    {9,  ee_interpreter::jalr},
    {15, ee_interpreter::sync},
    {16, ee_interpreter::mfhi},
};

static ee_op_template table1[] =
{
};

static ee_op_template table16[] =
{
    {0,  ee_interpreter::mfc0},
    {4,  ee_interpreter::mtc0},
    {16, ee_interpreter::run_table16_16},
};

static ee_op_template table16_16[] =
{
    {2, ee_interpreter::tlbwi},
};

void init_tables()
{
    for(int i = 0; i < 64; i++)
    {
        ee_interpreter::op_table[i] = ee_interpreter::unknown;
    }

    for(int i = 0; i < 64; i++)
    {
        ee_interpreter::op_table0[i] = ee_interpreter::unknown;
    }

    for(int i = 0; i < 32; i++)
    {
        ee_interpreter::op_table1[i] = ee_interpreter::unknown;
    }

    for(int i = 0; i < 32; i++)
    {
        ee_interpreter::op_table16[i] = ee_interpreter::unknown;
    }
    
    for(int i = 0; i < 64; i++)
    {
        ee_interpreter::op_table16_16[i] = ee_interpreter::unknown;
    }

    for(auto& tbl_op : primary_table)
    {
        ee_interpreter::op_table[tbl_op.opcode] = tbl_op.inst;
    }

    for(auto& tbl_op : table0)
    {
        ee_interpreter::op_table0[tbl_op.opcode] = tbl_op.inst;
    }

    for(auto& tbl_op : table1)
    {
        ee_interpreter::op_table1[tbl_op.opcode] = tbl_op.inst;
    }

    for(auto& tbl_op : table16)
    {
        ee_interpreter::op_table16[tbl_op.opcode] = tbl_op.inst;
    }
    
    for(auto& tbl_op : table16_16)
    {
        ee_interpreter::op_table16_16[tbl_op.opcode] = tbl_op.inst;
    }
}
}
}