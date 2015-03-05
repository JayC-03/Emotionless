#include "core/ee/ee.h"
#include "core/hle/elf.h"
#include "common/log.h"

void main_loop(int ac, char** av)
{
    EE::interpreter = new ee_interpreter();

    EE::interpreter->init();

#ifndef USE_HLE
    FILE* bios = fopen("bios.bin","rb");
    fread(Memory::bios,1,0x400000,bios);
    fclose(bios);
#else
    FILE* elffp = fopen(av[1],"rb");
    ELF::ELF elf;
    elf.load(elffp);
#endif

    for(int i = 0; i<3500; i++)
    {
        EE::interpreter->single_step();
        log_print("main", "v0: " + to_string(EE::ee_state.gpr[2].ud[0]), log_level::debug);
    }
}