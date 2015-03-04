#pragma once

#include "common/common.h"

#include "core/ee/ee_core_base.h"
#include "core/ee/ee_inst.h"

class ee_interpreter : public ee_core_base
{
public:
    void init() override;
    void shutdown() override;
    void single_step() override;

    static std::function<void(ee_inst)> op_table[64];
    static std::function<void(ee_inst)> op_table0[64];
    static std::function<void(ee_inst)> op_table1[32];
    static std::function<void(ee_inst)> op_table16[32];
    static std::function<void(ee_inst)> op_table16_16[64];

    static void unknown(ee_inst inst);

    static void run_table0(ee_inst inst);
    static void run_table1(ee_inst inst);
    static void run_table16(ee_inst inst);
    static void run_table16_16(ee_inst inst);

    static void sll(ee_inst inst);
    static void sync(ee_inst inst);
    static void mfc0(ee_inst inst);
    static void mtc0(ee_inst inst);
    static void blezl(ee_inst inst);
    static void bne(ee_inst inst);
    static void slti(ee_inst inst);
    static void ori(ee_inst inst);
    static void addiu(ee_inst inst);
    static void jr(ee_inst inst);
    static void jalr(ee_inst inst);
    static void lui(ee_inst inst);
    static void sw(ee_inst inst);
    static void sd(ee_inst inst);
    static void mfhi(ee_inst inst);
    static void tlbwi(ee_inst inst);
};