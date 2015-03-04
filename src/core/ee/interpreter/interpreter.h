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
    static std::function<void(ee_inst)> op_table16[32];

    static void unknown(ee_inst inst);

    static void run_table0(ee_inst inst);
    static void run_table16(ee_inst inst);

    static void sll(ee_inst inst);
};