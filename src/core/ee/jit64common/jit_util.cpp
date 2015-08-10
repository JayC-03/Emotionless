// Copyright 2008 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "common/common_types.h"

#include "core/ee/jit64common/jit_util.h"
#include "core/ee/jit64common/jit_base.h"

using namespace Gen;

void EmuCodeBlock::UnsafeLoadRegToReg(X64Reg reg_addr, X64Reg reg_value, int accessSize, s32 offset, bool signExtend)
{
	MOVZX(32, accessSize, reg_value, MComplex(RMEM, reg_addr, SCALE_1, offset));
	if (accessSize == 32)
	{
		BSWAP(32, reg_value);
	}
	else if (accessSize == 16)
	{
		BSWAP(32, reg_value);
		if (signExtend)
			SAR(32, R(reg_value), Imm8(16));
		else
			SHR(32, R(reg_value), Imm8(16));
	}
	else if (signExtend)
	{
		// TODO: bake 8-bit into the original load.
		MOVSX(32, accessSize, reg_value, R(reg_value));
	}
}

void EmuCodeBlock::UnsafeLoadRegToRegNoSwap(X64Reg reg_addr, X64Reg reg_value, int accessSize, s32 offset, bool signExtend)
{
	if (signExtend)
		MOVSX(32, accessSize, reg_value, MComplex(RMEM, reg_addr, SCALE_1, offset));
	else
		MOVZX(32, accessSize, reg_value, MComplex(RMEM, reg_addr, SCALE_1, offset));
}

u8 *EmuCodeBlock::UnsafeLoadToReg(X64Reg reg_value, OpArg opAddress, int accessSize, s32 offset, bool signExtend)
{
	u8 *result;
	OpArg memOperand;
	if (opAddress.IsSimpleReg())
	{
		// Deal with potential wraparound.  (This is just a heuristic, and it would
		// be more correct to actually mirror the first page at the end, but the
		// only case where it probably actually matters is JitIL turning adds into
		// offsets with the wrong sign, so whatever.  Since the original code
		// *could* try to wrap an address around, however, this is the correct
		// place to address the issue.)
		if ((u32) offset >= 0x1000)
		{
			LEA(32, reg_value, MDisp(opAddress.GetSimpleReg(), offset));
			opAddress = R(reg_value);
			offset = 0;
		}
		memOperand = MComplex(RMEM, opAddress.GetSimpleReg(), SCALE_1, offset);
	}
	else if (opAddress.IsImm())
	{
		MOV(32, R(reg_value), Imm32((u32)(opAddress.Imm32() + offset)));
		memOperand = MRegSum(RMEM, reg_value);
	}
	else
	{
		MOV(32, R(reg_value), opAddress);
		memOperand = MComplex(RMEM, reg_value, SCALE_1, offset);
	}

	result = GetWritableCodePtr();
	if (accessSize == 8 && signExtend)
		MOVSX(32, accessSize, reg_value, memOperand);
	else
		MOVZX(64, accessSize, reg_value, memOperand);

	switch (accessSize)
	{
	case 8:
		assert(BACKPATCH_SIZE - (GetCodePtr() - result <= 0));
		break;

	case 16:
		BSWAP(32, reg_value);
		if (signExtend)
			SAR(32, R(reg_value), Imm8(16));
		else
			SHR(32, R(reg_value), Imm8(16));
		break;

	case 32:
		BSWAP(32, reg_value);
		break;

	case 64:
		BSWAP(64, reg_value);
		break;
	}

	return result;
}

u8 *EmuCodeBlock::UnsafeWriteRegToReg(OpArg reg_value, X64Reg reg_addr, int accessSize, s32 offset, bool swap)
{
	u8* result = GetWritableCodePtr();
	OpArg dest = MComplex(RMEM, reg_addr, SCALE_1, offset);
	if (reg_value.IsImm())
	{
		if (swap)
			reg_value = SwapImmediate(accessSize, reg_value);
		MOV(accessSize, dest, reg_value);
	}
	else if (swap)
	{
		if (accessSize > 8)
			BSWAP(accessSize, reg_value.GetSimpleReg());
		result = GetWritableCodePtr();
		MOV(accessSize, dest, reg_value);
	}
	else
	{
		MOV(accessSize, dest, reg_value);
	}

	return result;
}

static OpArg FixImmediate(int accessSize, OpArg arg)
{
	if (arg.IsImm())
	{
		arg = accessSize == 8  ? Imm8((u8)arg.Imm32()) :
		      accessSize == 16 ? Imm16((u16)arg.Imm32()) :
		                         Imm32((u32)arg.Imm32());
	}
	return arg;
}

// Abstract between AVX and SSE: automatically handle 3-operand instructions
void EmuCodeBlock::avx_op(void (XEmitter::*avxOp)(X64Reg, X64Reg, const OpArg&), void (XEmitter::*sseOp)(X64Reg, const OpArg&),
                          X64Reg regOp, const OpArg& arg1, const OpArg& arg2, bool packed, bool reversible)
{
	if (arg1.IsSimpleReg() && regOp == arg1.GetSimpleReg())
	{
		(this->*sseOp)(regOp, arg2);
	}
	else if (arg1.IsSimpleReg() && cpu_info.bAVX)
	{
		(this->*avxOp)(regOp, arg1.GetSimpleReg(), arg2);
	}
	else if (arg2.IsSimpleReg() && arg2.GetSimpleReg() == regOp)
	{
		if (reversible)
		{
			(this->*sseOp)(regOp, arg1);
		}
		else
		{
			// The ugly case: regOp == arg2 without AVX, or with arg1 == memory
			if (!arg1.IsSimpleReg() || arg1.GetSimpleReg() != XMM0)
				MOVAPD(XMM0, arg1);
			if (cpu_info.bAVX)
			{
				(this->*avxOp)(regOp, XMM0, arg2);
			}
			else
			{
				(this->*sseOp)(XMM0, arg2);
				if (packed)
					MOVAPD(regOp, R(XMM0));
				else
					MOVSD(regOp, R(XMM0));
			}
		}
	}
	else
	{
		if (packed)
			MOVAPD(regOp, arg1);
		else
			MOVSD(regOp, arg1);
		(this->*sseOp)(regOp, arg1 == arg2 ? R(regOp) : arg2);
	}
}

// Abstract between AVX and SSE: automatically handle 3-operand instructions
void EmuCodeBlock::avx_op(void (XEmitter::*avxOp)(X64Reg, X64Reg, const OpArg&, u8), void (XEmitter::*sseOp)(X64Reg, const OpArg&, u8),
                          X64Reg regOp, const OpArg& arg1, const OpArg& arg2, u8 imm)
{
	if (arg1.IsSimpleReg() && regOp == arg1.GetSimpleReg())
	{
		(this->*sseOp)(regOp, arg2, imm);
	}
	else if (arg1.IsSimpleReg() && cpu_info.bAVX)
	{
		(this->*avxOp)(regOp, arg1.GetSimpleReg(), arg2, imm);
	}
	else if (arg2.IsSimpleReg() && arg2.GetSimpleReg() == regOp)
	{
		// The ugly case: regOp == arg2 without AVX, or with arg1 == memory
		if (!arg1.IsSimpleReg() || arg1.GetSimpleReg() != XMM0)
			MOVAPD(XMM0, arg1);
		if (cpu_info.bAVX)
		{
			(this->*avxOp)(regOp, XMM0, arg2, imm);
		}
		else
		{
			(this->*sseOp)(XMM0, arg2, imm);
			MOVAPD(regOp, R(XMM0));
		}
	}
	else
	{
		MOVAPD(regOp, arg1);
		(this->*sseOp)(regOp, arg1 == arg2 ? R(regOp) : arg2, imm);
	}
}
