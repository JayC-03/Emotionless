#include "core/main.h"
#include "core/ee/ee.h"
#include "core/memmap.h"
#include "core/hle/elf.h"
#include "common/log.h"

void main_loop(std::string fn)
{
    EE::interpreter = new ee_interpreter();

    EE::interpreter->init();

#ifndef USE_BIOS_HLE
    FILE* bios = fopen(fn.c_str(),"rb");
    fread(Memory::bios,1,0x400000,bios);
    fclose(bios);
#else
    FILE* elffp = fopen(fn.c_str(),"rb");
    ELF::ELF elf;
    elf.load(elffp);
#endif

	for(int i = 0; i < 8000; i++)
	{
		EE::interpreter->single_step();
	}

	delete EE::interpreter;
}
