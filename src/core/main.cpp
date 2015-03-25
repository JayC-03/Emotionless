#include "core/main.h"
#include "core/ee/ee.h"
#include "core/iop/iop.h"
#include "core/memmap.h"
#include "core/hle/elf.h"

#ifdef USE_BIOS_HLE
#include "core/hle/bios_hle.h"
#endif

#include "core/hw/dmac.h"

#include "common/log.h"

void main_loop(std::string fn, std::string fn2)
{
    EE::interpreter = new ee_interpreter();
    IOP::interpreter = new iop_interpreter();

    EE::interpreter->init();
    IOP::interpreter->init();
    DMAC::init_dmac_channels();

#ifdef USE_BIOS_HLE
    HLE::init_syscalls();
#endif

    FILE* bios = fopen(fn.c_str(),"rb");
    fread(MemoryEE::bios,1,0x400000,bios);
    fclose(bios);
#ifdef USE_BIOS_HLE
    FILE* elffp = fopen(fn2.c_str(),"rb");
    ELF::ELF elf;
    elf.load(elffp);
#endif

    for(int i = 0;i < 2;i++)
    {
        EE::interpreter->run();
        DMAC::run();

#ifndef USE_BIOS_HLE
        if((i & 3) == 0) IOP::interpreter->single_step();
#endif
    }

    delete EE::interpreter;
    delete IOP::interpreter;
}
