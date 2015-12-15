#include "core/ee/ee.h"
#include "core/ee/jit64/jit.h"

void EEJit64::init()
{
  gpr.SetEmitter(this);
  fpr.SetEmitter(this);

  AllocCodeSpace(32 * 1024 * 1024);

  blocks.Init();

  farcode.Init(2 * 1024 * 1024);
}

void EEJit64::clear_cache()
{
  blocks.Clear();
  farcode.ClearCodeSpace();
  ClearCodeSpace();
}

void EEJit64::shutdown()
{
  FreeCodeSpace();
  blocks.Shutdown();
  farcode.Shutdown();
}

void EEJit64::FallBackToInterpreter(ee_inst _inst)
{
  gpr.Flush();
	fpr.Flush();
	EE::interpreter::interpreter_func_type instr = ee_interpreter::op_table[inst.opcd];
	ABI_PushRegistersAndAdjustStack({}, 0);
	ABI_CallFunctionC((void*)instr, inst.hex);
	ABI_PopRegistersAndAdjustStack({}, 0);
}

void EEJit64::Jit(u32 em_address)
{
  int block_num = blocks.AllocateBlock(em_address);
	JitBlock *b = blocks.GetBlock(block_num);

  while(!EE::ee_state.jump)
  {
    ee_inst inst_code;
    inst_code.hex = EE::Read32(js.compilerPC);
    FallBackToInterpreter(inst_code);
    js.compilerPC+=4;
  }

  if(EE::ee_state.jump_likely)
  {
      if(EE::ee_state.condition) EE::ee_state.branch = 1;
      else js.compilerPC+=4;
  }
  else
  {
      if(EE::ee_state.condition) EE::ee_state.branch = 1;
  }

  if(EE::ee_state.branch) js.compilerPC = EE::ee_state.jump_target;

  MOV(32, &EE::ee_state.pc, Imm32(js.compilerPC));

  blocks.FinalizeBlock(block_num, jo.enableBlocklink, GetCodePtr());
}

void EEJit64::run()
{
	u8* codeptr = GetCodePtr();
  CompiledCode code = (CompiledCode)codeptr;
  code();
}
