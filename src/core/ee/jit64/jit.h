#pragma once

#include "common/emitters/x64_emitter.h"

#include "core/memmap.h"
#include "core/ee/jit64/jit_reg_cache.h"
#include "core/ee/jit64common/jit_util.h"
#include "core/ee/jit64common/jit_base.h"
#include "core/ee/jit64common/jit_cache.h"

class Jit64 : public Jitx86Base
{
private:

	GPRRegCache gpr;
	FPURegCache fpr;

public:
	Jit64() {}
	~Jit64() {}

	void init() override;

	void shutdown() override;

	// Jit!

	void Jit(u32 em_address) override;

	JitBlockCache *GetBlockCache() override { return &blocks; }

	void clear_cache() override;

	// Run!
	void Run() override;
	void SingleStep() override;

	// OPCODES
	void FallBackToInterpreter(ee_inst _inst);
}
