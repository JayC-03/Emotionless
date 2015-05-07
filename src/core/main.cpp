#include "core/main.h"
#include "core/ee/ee.h"
#ifdef USE_JIT
#include "core/ee/jit/jit-x64/jit_x64.h"
#endif
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

	log_filter = error | warning | debug;

    EE::interpreter->init();
#ifdef USE_JIT
	ee_jit_x64 ee_jit;
	ee_jit.init();
#endif
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
#ifdef USE_JIT
		ee_jit.compile();
        ee_jit.run();
#else
		EE::interpreter->run();
#endif
        DMAC::run();

#ifndef USE_BIOS_HLE
        if((i & 3) == 0) IOP::interpreter->single_step();
#endif
    }

#ifdef USE_JIT
	ee_jit.shutdown();
#endif

    delete EE::interpreter;
    delete IOP::interpreter;
}
