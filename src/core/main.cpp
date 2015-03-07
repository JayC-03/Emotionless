#include "core/main.h"
#include "core/ee/ee.h"
#include "core/memmap.h"
#include "core/hle/elf.h"
#include "common/log.h"

bool main_quit;
bool main_break;
int main_break_steps;

void main_loop(std::string fn)
{
    EE::interpreter = new ee_interpreter();

    EE::interpreter->init();

#ifndef USE_BIOS_HLE
    FILE* bios = fopen("bios.bin","rb");
    fread(Memory::bios,1,0x400000,bios);
    fclose(bios);
#else
    FILE* elffp = fopen(fn.c_str(),"rb");
    ELF::ELF elf;
    elf.load(elffp);
#endif

    while(!main_quit)
	{
        if(!main_break) EE::interpreter->single_step();
	else
	{
		for(;main_break_steps != 0;main_break_steps--)
		{
			EE::interpreter->single_step();
		}
	}
        //log_print("main", "v0: " + to_string(EE::ee_state.gpr[2].ud[0]), log_level::debug);
    }

	delete EE::interpreter;
}
