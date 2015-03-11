#include "core/iop/iop.h"
#include "core/iop/interpreter/interpreter.h"

namespace IOP
{

iop_state_t iop_state;

iop_interpreter* interpreter;

u32 TranslateAddress(u32 virt_addr)
{
    //TODO: HACKY
    if(virt_addr >= 0x70000000 && virt_addr < 0x70004000) return (virt_addr - 0x6e000000);
    if(virt_addr >= 0x30100000 && virt_addr < 0x32000000) return (virt_addr - 0x30000000);
    return virt_addr & 0x1fffffff;
}

u8 Read8(u32 virt_addr)
{
    return MemoryIOP::Read8(TranslateAddress(virt_addr));
}

u32 Read32(u32 virt_addr)
{
    return MemoryIOP::Read32(TranslateAddress(virt_addr));
}

void Write8(u32 virt_addr, u8 data)
{
    MemoryIOP::Write8(TranslateAddress(virt_addr),data);
}

void Write32(u32 virt_addr, u32 data)
{
    MemoryIOP::Write32(TranslateAddress(virt_addr),data);
}

}
