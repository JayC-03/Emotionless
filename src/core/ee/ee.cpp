#include "core/ee/ee.h"
#include "core/ee/interpreter/interpreter.h"

namespace EE
{

ee_state_t ee_state;

ee_interpreter* interpreter;

u32 TranslateAddress(u32 virt_addr)
{
    //Checking Status.KSU
    switch((ee_state.cop0r[12] >> 3) & 3)
    {
    case 0:
    {
        //TODO: HACKY
        if(virt_addr >= 0x80000000 && virt_addr < 0xC0000000)
        {
            return virt_addr & 0x1FFFFFFF;
        }
        break;
    }
    default:
    {
        log_print("EE", "Unknown/Unimplemented Status.KSU state " + to_string((ee_state.cop0r[12] >> 3) & 3), log_level::warning);
        break;
    }
    }
    return 0;
}

u32 Read32(u32 virt_addr)
{
    Memory::Read32(TranslateAddress(virt_addr));
}

}
