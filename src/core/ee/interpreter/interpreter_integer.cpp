#include "core/ee/interpreter/interpreter.h"
#include "core/ee/ee.h"

void ee_interpreter::sll(ee_inst inst)
{
    rGPR[inst.RD].ud[0] = (s64)(s32)(rGPR[inst.RT].ul[0] << inst.SA);
}

void ee_interpreter::srl(ee_inst inst)
{
    rGPR[inst.RD].ud[0] = (s64)(s32)(rGPR[inst.RT].ul[0] >> inst.SA);
}

void ee_interpreter::sra(ee_inst inst)
{
    rGPR[inst.RD].ud[0] = (s64)(s32)((s32)rGPR[inst.RT].ul[0] >> inst.SA);
}

void ee_interpreter::slt(ee_inst inst)
{
    rGPR[inst.RD].ud[1] = 0;
    if(rGPR[inst.RS].sd[0] < rGPR[inst.RT].sd[0]) rGPR[inst.RD].ud[0] = 1;
    else rGPR[inst.RD].ud[0] = 0;
}

void ee_interpreter::sltu(ee_inst inst)
{
    rGPR[inst.RD].ud[1] = 0;
    if(rGPR[inst.RS].ud[0] < rGPR[inst.RT].ud[0]) rGPR[inst.RD].ud[0] = 1;
    else rGPR[inst.RD].ud[0] = 0;
}



void ee_interpreter::slti(ee_inst inst)
{
    rGPR[inst.RT].ud[1] = 0;
    if(rGPR[inst.RS].sd[0] < (s64)inst.SIMM_16) rGPR[inst.RT].ud[0] = 1;
    else rGPR[inst.RT].ud[0] = 0;
}

void ee_interpreter::sltiu(ee_inst inst)
{
    rGPR[inst.RT].ud[1] = 0;
    if(rGPR[inst.RS].ud[0] < (u64)(s64)inst.SIMM_16) rGPR[inst.RT].ud[0] = 1;
    else rGPR[inst.RT].ud[0] = 0;
}

void ee_interpreter::and_ee(ee_inst inst)
{
    rGPR[inst.RD].ud[0] = rGPR[inst.RS].ud[0] & rGPR[inst.RT].ud[0];
}

void ee_interpreter::andi(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = rGPR[inst.RS].ud[0] & inst.UIMM_16;
}

void ee_interpreter::ori(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = rGPR[inst.RS].ud[0] | inst.UIMM_16;
}

void ee_interpreter::or_ee(ee_inst inst)
{
    rGPR[inst.RD].ud[0] = rGPR[inst.RS].ud[0] | rGPR[inst.RT].ud[0];
}

void ee_interpreter::mult(ee_inst inst)
{
    u64 prod = rGPR[inst.RS].ul[0] * rGPR[inst.RT].ul[0];
    rLO.ud[0] = (s64)(s32)(u32)(prod & 0xffffffff);
    rHI.ud[0] = (s64)(s32)(u32)(prod >> 32);
}

void ee_interpreter::divu(ee_inst inst)
{
    //TODO: Figure out what divide by zero does. The manual says it doesn't raise an exception.
    if(rGPR[inst.RT].ul[0] == 0) return;
    u32 quotient = rGPR[inst.RS].ul[0] / rGPR[inst.RT].ul[0];
    u32 remainder = rGPR[inst.RS].ul[0] % rGPR[inst.RT].ul[0];
    rLO.ud[0] = (s64)(s32)quotient;
    rHI.ud[0] = (s64)(s32)remainder;
}

void ee_interpreter::addi(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = (s64)(rGPR[inst.RS].sl[0] + (s32)inst.SIMM_16);
}

void ee_interpreter::addu(ee_inst inst)
{
    rGPR[inst.RD].ud[0] = (s64)(s32)(u32)(rGPR[inst.RS].ud[0] + rGPR[inst.RT].ud[0]);
}

void ee_interpreter::addiu(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = (s64)(rGPR[inst.RS].sl[0] + (s32)inst.SIMM_16);
}

void ee_interpreter::daddu(ee_inst inst)
{
    rGPR[inst.RD].ud[0] = rGPR[inst.RS].ud[0] + rGPR[inst.RT].ud[0];
}

void ee_interpreter::daddi(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = rGPR[inst.RS].ud[0] + (s64)inst.SIMM_16;
}

void ee_interpreter::mfhi(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = rHI.ud[0];
}

void ee_interpreter::mflo(ee_inst inst)
{
    rGPR[inst.RT].ud[0] = rLO.ud[0];
}

void ee_interpreter::sync(ee_inst inst)
{
    //TODO: This doesn't need to be implemented yet.
}

void ee_interpreter::subu(ee_inst inst)
{
    rGPR[inst.RD].ud[0] = (s64)(s32)(u32)(rGPR[inst.RS].ud[0] - rGPR[inst.RT].ud[0]);
}

void ee_interpreter::movn(ee_inst inst)
{
    if(rGPR[inst.RT].ud[0] != 0) rGPR[inst.RD].ud[0] = rGPR[inst.RS].ud[0];
}
