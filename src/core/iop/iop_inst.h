#pragma once

union iop_inst
{
    u32 hex;

    iop_inst(u32 _hex) {
        hex = _hex;
    }
    iop_inst() {
        hex = 0;
    }

    struct
    {
        u32 funct : 6;
        u32 SA : 5;
        u32 RD : 5;
        u32 RT : 5;
        u32 RS : 5;
        u32 opcd : 6;
    };

    struct
    {
        signed SIMM_16 : 16;
        u32 : 16;
    };

    struct
    {
        u32 UIMM_16 : 16;
        u32 : 16;
    };
    
    struct
    {
        u32 instr_index : 26;
        u32 : 6;
    };
};
