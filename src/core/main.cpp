#include "core/ee/ee.h"

void main_loop()
{
    FILE* bios = fopen("bios.bin","rb");
    fread(Memory::bios,1,0x400000,bios);
    fclose(bios);

    EE::interpreter = new ee_interpreter();

    EE::interpreter->init();

    for(int i = 0; i<50; i++)
    {
        EE::interpreter->single_step();
    }
}