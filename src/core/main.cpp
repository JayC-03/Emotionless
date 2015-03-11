#include "core/main.h"
#include "core/ee/ee.h"
#include "core/iop/iop.h"
#include "core/memmap.h"
#include "core/hle/elf.h"
#include "common/log.h"

void main_loop(std::string fn, std::string fn2)
{
    EE::interpreter = new ee_interpreter();
	IOP::interpreter = new iop_interpreter();

    EE::interpreter->init();
	IOP::interpreter->init();

    FILE* bios = fopen(fn.c_str(),"rb");
    fread(MemoryEE::bios,1,0x400000,bios);
    fclose(bios);
#ifdef USE_BIOS_HLE
    FILE* elffp = fopen(fn2.c_str(),"rb");
    ELF::ELF elf;
    elf.load(elffp);
#endif

	for(int i = 0; i < 8000; i++)
	{
		EE::interpreter->single_step();
#ifndef USE_BIOS_HLE
		if((i & 3) == 0) IOP::interpreter->single_step();
#endif
	}

	delete EE::interpreter;
	delete IOP::interpreter;
}
