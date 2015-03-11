#pragma once

#include "common/common.h"

#include "core/iop/iop_core_base.h"
#include "core/iop/iop_inst.h"

class iop_interpreter : public iop_core_base
{
public:
    void init() override;
    void shutdown() override;
    void single_step() override;

    static std::function<void(iop_inst)> op_table[64];
    static std::function<void(iop_inst)> op_table0[64];
    static std::function<void(iop_inst)> op_table1[32];
    static std::function<void(iop_inst)> op_table16[32];
    static std::function<void(iop_inst)> op_table16_16[64];

    static void unknown(iop_inst inst);

	static void exception();

    static void run_table0(iop_inst inst);
    static void run_table1(iop_inst inst);
    static void run_table16(iop_inst inst);
    static void run_table16_16(iop_inst inst);

	static void break_iop(iop_inst inst);
    static void sll(iop_inst inst);
    static void srl(iop_inst inst);
	static void sra(iop_inst inst);
    static void sync(iop_inst inst);
    static void mfc0(iop_inst inst);
    static void mtc0(iop_inst inst);
    static void bne(iop_inst inst);
    static void beq(iop_inst inst);
	static void bgez(iop_inst inst);
	static void blez(iop_inst inst);
    static void bnel(iop_inst inst);
    static void beql(iop_inst inst);
	static void blezl(iop_inst inst);
    static void slt(iop_inst inst);
    static void sltu(iop_inst inst);
    static void slti(iop_inst inst);
    static void sltiu(iop_inst inst);
	static void andi(iop_inst inst);
    static void and_iop(iop_inst inst);
    static void ori(iop_inst inst);
    static void or_iop(iop_inst inst);
	static void addi(iop_inst inst);
	static void addu(iop_inst inst);
    static void addiu(iop_inst inst);
    static void jr(iop_inst inst);
    static void jalr(iop_inst inst);
    static void jal(iop_inst inst);
    static void lb(iop_inst inst);
	static void lbu(iop_inst inst);
    static void lw(iop_inst inst);
    static void lui(iop_inst inst);
    static void sb(iop_inst inst);
    static void sw(iop_inst inst);
    static void tlbwi(iop_inst inst);
};
