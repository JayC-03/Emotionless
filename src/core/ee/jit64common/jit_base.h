// Copyright 2010 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

//#define JIT_LOG_X86     // Enables logging of the generated x86 code
//#define JIT_LOG_GPR     // Enables logging of the PPC general purpose regs
//#define JIT_LOG_FPR     // Enables logging of the PPC floating point regs

#include <unordered_set>

#include "common/emitters/x64_emitter.h"

#include "core/memmap.h"
#include "core/ee/ee_core_base.h"
#include "core/ee/ee.h"
#include "core/ee/jit64common/jit_util.h"
#include "core/ee/jit64common/jit_cache.h"

// TODO: find a better place for x86-specific stuff
// The following register assignments are common to Jit64 and Jit64IL:
// RSCRATCH and RSCRATCH2 are always scratch registers and can be used without
// limitation.
#define RSCRATCH RAX
#define RSCRATCH2 RDX
// RSCRATCH_EXTRA may be in the allocation order, so it has to be flushed
// before use.
#define RSCRATCH_EXTRA RCX
// RMEM points to the start of emulated memory.
#define RMEM RBX

// Use these to control the instruction selection
// #define INSTRUCTION_START FallBackToInterpreter(inst); return;
// #define INSTRUCTION_START PPCTables::CountInstruction(inst);
#define INSTRUCTION_START

#define FALLBACK_IF(cond) do { if (cond) { FallBackToInterpreter(inst); return; } } while (0)

#define JITDISABLE(setting)

class JitBase : public ee_core_base
{
protected:
    struct JitOptions
    {
        bool fastmem;
        bool memcheck;
        bool alwaysUseMemFuncs;
    };
    struct JitState
    {
        u32 compilerPC;
        u32 blockStart;
        int instructionNumber;
        int instructionsLeft;
        int downcountAmount;
        u32 numLoadStoreInst;
        u32 numFloatingPointInst;
        // If this is set, we need to generate an exception handler for the fastmem load.
        u8* fastmemLoadStore;
        // If this is set, a load or store already prepared a jump to the exception handler for us,
        // so just fixup that branch instead of testing for a DSI again.
        bool fixupExceptionHandler;
        Gen::FixupBranch exceptionHandler;
        // If these are set, we've stored the old value of a register which will be loaded in revertLoad,
        // which lets us revert it on the exception path.
        int revertGprLoad;
        int revertFprLoad;

        bool assumeNoPairedQuantize;
        bool firstFPInstructionFound;
        bool isLastInstruction;
        int skipInstructions;
        bool carryFlagSet;
        bool carryFlagInverted;

        int fifoBytesThisBlock;

        ee_inst op;
        u8* rewriteStart;

        JitBlock *curBlock;
    };

public:
    // This should probably be removed from public:
    JitOptions jo;
    JitState js;

    virtual JitBaseBlockCache *GetBlockCache() = 0;

    virtual void Jit(u32 em_address) = 0;
};

class Jitx86Base : public JitBase, public EmuCodeBlock
{
protected:
    JitBlockCache blocks;
public:
    JitBlockCache *GetBlockCache() override {
        return &blocks;
    }
};

extern JitBase *jit;

void Jit(u32 em_address);

// Merged routines that should be moved somewhere better
u32 Helper_Mask(u8 mb, u8 me);
