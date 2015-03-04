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
    {16, ee_interpreter::run_table16},
};

static ee_op_template table0[] =
{
    {0, ee_interpreter::sll},
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
        ee_interpreter::op_table16[i] = ee_interpreter::unknown;
    }

    for(auto& tbl_op : primary_table)
    {
        ee_interpreter::op_table[tbl_op.opcode] = tbl_op.inst;
    }

    for(auto& tbl_op : table0)
    {
        ee_interpreter::op_table0[tbl_op.opcode] = tbl_op.inst;
    }
}
}
}