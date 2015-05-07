#pragma once

#include "common/common.h"

#include "core/ee/ee_core_base.h"
#include "core/ee/ee_inst.h"
#include "core/ee/interpreter/interpreter.h"

#include "asmjit.h"

using namespace asmjit;
using namespace asmjit::x86;

class ee_jit_x64 : public ee_core_base
{
public:
	bool end_block;

    void init() override;
    void shutdown() override;
	void compile() override;
	void run() override;

	static void fallback(ee_inst inst);

	static void lui(ee_inst inst);

	static interpreter_func_type op_table[64];
};

extern JitRuntime runtime;
extern X86Compiler* compiler;
