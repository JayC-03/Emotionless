#include "core/ee/ee.h"
#include "core/ee/jit64/jit.h"

void Jit64::init()
{
  gpr.SetEmitter(this);
  fpr.SetEmitter(this);

  AllocCodeSpace(32 * 1024 * 1024);

  blocks.Init();

  farcode.Init(2 * 1024 * 1024);
}

void Jit64::clear_cache()
{
  blocks.Clear();
  farcode.ClearCodeSpace();
  ClearCodeSpace();
}

void Jit64::shutdown()
{
  FreeCodeSpace();
  blocks.Shutdown();
  farcode.Shutdown();
}

void Jit64::FallBackToInterpreter(ee_inst _inst)
{
  gpr.Flush();
	fpr.Flush();
	if (js.isLastInstruction)
	{
		MOV(32, &EE::ee_state.pc, Imm32(js.compilerPC));
	}
	EE::interpreter::interpreter_func_type instr = ee_interpreter::op_table[inst.opcd];
	ABI_PushRegistersAndAdjustStack({}, 0);
	ABI_CallFunctionC((void*)instr, inst.hex);
	ABI_PopRegistersAndAdjustStack({}, 0);
}
