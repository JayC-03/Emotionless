#pragma once

#include "common/common.h"

#include "core/ee/ee_core_base.h"
#include "core/ee/ee_inst.h"

class ee_interpreter : public ee_core_base
{
public:
	bool end_block;

    void init() override;
    void shutdown() override;
    void single_step() override;
	void run() override;

    static std::function<void(ee_inst)> op_table[64];
    static std::function<void(ee_inst)> op_table0[64];
    static std::function<void(ee_inst)> op_table1[32];
    static std::function<void(ee_inst)> op_table16[32];
    static std::function<void(ee_inst)> op_table16_16[64];
    static std::function<void(ee_inst)> op_table28[64];
    static std::function<void(ee_inst)> op_table28_40[32];

    static void unknown(ee_inst inst);

    static void exception();

    static void run_table0(ee_inst inst);
    static void run_table1(ee_inst inst);
    static void run_table16(ee_inst inst);
    static void run_table16_16(ee_inst inst);
    static void run_table28(ee_inst inst);
    static void run_table28_40(ee_inst inst);

    static void syscall(ee_inst inst);
    static void break_ee(ee_inst inst);
    static void sll(ee_inst inst);
    static void dsll(ee_inst inst);
    static void dsll32(ee_inst inst);
    static void srl(ee_inst inst);
	static void dsrl(ee_inst inst);
    static void dsrl32(ee_inst inst);
    static void sra(ee_inst inst);
    static void dsra32(ee_inst inst);
    static void sync(ee_inst inst);
    static void mfc0(ee_inst inst);
    static void mtc0(ee_inst inst);
    static void swc1(ee_inst inst);
    static void bne(ee_inst inst);
    static void beq(ee_inst inst);
	static void bltz(ee_inst inst);
    static void bgez(ee_inst inst);
    static void blez(ee_inst inst);
	static void bgtz(ee_inst inst);
    static void bnel(ee_inst inst);
    static void beql(ee_inst inst);
    static void blezl(ee_inst inst);
    static void slt(ee_inst inst);
    static void sltu(ee_inst inst);
    static void slti(ee_inst inst);
    static void sltiu(ee_inst inst);
    static void andi(ee_inst inst);
    static void and_ee(ee_inst inst);
    static void ori(ee_inst inst);
    static void or_ee(ee_inst inst);
    static void xori(ee_inst inst);
    static void mult(ee_inst inst);
    static void mult1(ee_inst inst);
    static void div(ee_inst inst);
    static void divu(ee_inst inst);
    static void addi(ee_inst inst);
    static void addu(ee_inst inst);
    static void addiu(ee_inst inst);
    static void daddu(ee_inst inst);
    static void daddi(ee_inst inst);
    static void daddiu(ee_inst inst);
    static void subu(ee_inst inst);
    static void jr(ee_inst inst);
    static void j(ee_inst inst);
    static void jalr(ee_inst inst);
    static void jal(ee_inst inst);
    static void lb(ee_inst inst);
    static void lbu(ee_inst inst);
    static void lhu(ee_inst inst);
    static void lw(ee_inst inst);
    static void lwu(ee_inst inst);
    static void ld(ee_inst inst);
    static void lui(ee_inst inst);
    static void sb(ee_inst inst);
    static void sh(ee_inst inst);
    static void sw(ee_inst inst);
    static void sd(ee_inst inst);
    static void sq(ee_inst inst);
    static void mfhi(ee_inst inst);
    static void mflo(ee_inst inst);
    static void tlbwi(ee_inst inst);
	static void paddub(ee_inst inst);
    static void padduw(ee_inst inst);
    static void movn(ee_inst inst);
    static void ei(ee_inst inst);
    static void di(ee_inst inst);
};
