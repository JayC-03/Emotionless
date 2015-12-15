// Copyright 2010 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "common/bit_set.h"
#include "common/cpu_detect.h"
#include "common/emitters/x64_emitter.h"

// A place to throw blocks of code we don't want polluting the cache, e.g. rarely taken
// exception branches.
class FarCodeCache : public Gen::x64_code_block
{
private:
    bool m_enabled = false;
public:
    bool Enabled() {
        return m_enabled;
    }
    void Init(int size) {
        alloc_code_space(size);
        m_enabled = true;
    }
    void Shutdown() {
        free_code_space();
        m_enabled = false;
    }
};

static const int CODE_SIZE = 1024 * 1024 * 32;

// Like XCodeBlock but has some utilities for memory access.
class EmuCodeBlock : public Gen::x64_code_block
{
public:
    FarCodeCache farcode;
    u8* nearcode; // Backed up when we switch to far code.

    // Simple functions to switch between near and far code emitting
    void SwitchToFarCode()
    {
        nearcode = GetWritableCodePtr();
        SetCodePtr(farcode.GetWritableCodePtr());
    }

    void SwitchToNearCode()
    {
        farcode.SetCodePtr(GetWritableCodePtr());
        SetCodePtr(nearcode);
    }

    void UnsafeLoadRegToReg(Gen::X64Reg reg_addr, Gen::X64Reg reg_value, int accessSize, s32 offset = 0, bool signExtend = false);
    void UnsafeLoadRegToRegNoSwap(Gen::X64Reg reg_addr, Gen::X64Reg reg_value, int accessSize, s32 offset, bool signExtend = false);
    // these return the address of the MOV, for backpatching
    u8 *UnsafeWriteRegToReg(Gen::OpArg reg_value, Gen::X64Reg reg_addr, int accessSize, s32 offset = 0, bool swap = true);
    u8 *UnsafeWriteRegToReg(Gen::X64Reg reg_value, Gen::X64Reg reg_addr, int accessSize, s32 offset = 0, bool swap = true)
    {
        return UnsafeWriteRegToReg(R(reg_value), reg_addr, accessSize, offset, swap);
    }
    u8 *UnsafeLoadToReg(Gen::X64Reg reg_value, Gen::OpArg opAddress, int accessSize, s32 offset, bool signExtend);

    // applies to safe and unsafe WriteRegToReg
    bool WriteClobbersRegValue(int accessSize, bool swap)
    {
        return swap && accessSize > 8;
    }

    void avx_op(void (Gen::XEmitter::*avxOp)(Gen::X64Reg, Gen::X64Reg, const Gen::OpArg&), void (Gen::XEmitter::*sseOp)(Gen::X64Reg, const Gen::OpArg&),
                Gen::X64Reg regOp, const Gen::OpArg& arg1, const Gen::OpArg& arg2, bool packed = true, bool reversible = false);
    void avx_op(void (Gen::XEmitter::*avxOp)(Gen::X64Reg, Gen::X64Reg, const Gen::OpArg&, u8), void (Gen::XEmitter::*sseOp)(Gen::X64Reg, const Gen::OpArg&, u8),
                Gen::X64Reg regOp, const Gen::OpArg& arg1, const Gen::OpArg& arg2, u8 imm);
protected:
    std::unordered_map<u8 *, BitSet32> registersInUseAtLoc;
    std::unordered_map<u8 *, u32> pcAtLoc;
    std::unordered_map<u8 *, u8 *> exceptionHandlerAtLoc;
};
