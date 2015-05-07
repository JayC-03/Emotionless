#include "core/ee/interpreter/interpreter.h"
#include "core/ee/jit/jit-x64/jit_x64.h"
#include "core/ee/ee.h"

#include "core/main.h"

#include "common/log.h"

JitRuntime runtime;
X86Compiler* compiler;

interpreter_func_type ee_jit_x64::op_table[64] = {
    ee_jit_x64::fallback, //0
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback, //10
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::lui,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback, //20
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback, //30
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback, //40
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback, //50
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback, //60
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
    ee_jit_x64::fallback,
};

void ee_jit_x64::init()
{
    PC = 0xbfc00000;
    rCOP0[EE::COP0_regs::Cause] = 0;
    rCOP0[EE::COP0_regs::Status] = 0x000000;
    rCOP0[15] = 0x00002e20; //TODO: Value taken from PCSX2. VERIFY
    EE::ee_state.jump = 0;
    EE::ee_state.branch = 0;

    compiler = new X86Compiler(&runtime);
}

void ee_jit_x64::shutdown()
{
    delete compiler;
}

void ee_jit_x64::run()
{
    typedef void (*jit_func_type)(void);
    jit_func_type func = asmjit_cast<jit_func_type>(compiler->make());

    func();

    runtime.release((void*)func);

    log_print("EE JIT", "PC: " + to_string(PC), log_level::debug);
}

void ee_jit_x64::compile()
{
    compiler->addFunc(kFuncConvHost, FuncBuilder0<void>());

    ee_inst inst_code;

    while(true)
    {

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

        log_print("EE JIT", "PC: " + to_string(PC), log_level::debug);

        inst_code.hex = EE::Read32(PC);

        PC+=4;

        log_print("EE JIT", "Instruction: " + to_string(inst_code.hex), log_level::debug);

        log_print("EE JIT", "Instruction RS: " + to_string(inst_code.RS), log_level::verbose);

        log_print("EE JIT", "Instruction RT: " + to_string(inst_code.RT), log_level::verbose);

        log_print("EE JIT", "Instruction RD: " + to_string(inst_code.RD), log_level::verbose);

        log_print("EE JIT", "Instruction Opcode: " + to_string(inst_code.opcd), log_level::debug);

        //check if branch
        switch(inst_code.opcd)
        {
        case 0:
        {
            switch(inst_code.funct)
            {
            case 8:
            case 9:
            {
                goto loopdone;
            }
            }
            break;
        }
        case 1:
        {
            switch(inst_code.RT)
            {
            case 0 ... 3:
            case 16 ... 19:
            {
                goto loopdone;
            }
            }
            break;
        }
        case 2 ... 7:
        case 20 ... 23:
        {
            goto loopdone;
        }
        }

        fallback(inst_code);
    }

loopdone:

    fallback(inst_code);

    compiler->ret();
    compiler->endFunc();
}

void ee_jit_x64::fallback(ee_inst inst)
{
    //We cheat here by just passing ee_inst::hex and letting the constructor figure it out.
    X86CallNode* callnode = compiler->call(imm_ptr(reinterpret_cast<void*>(ee_interpreter::op_table[inst.opcd])), kFuncConvHost, FuncBuilder1<void, u32>());
    callnode->setArg(0,imm_u(inst.hex));
}
