#include "core/memmap.h"
#include "core/hw/gif.h"
#include "common/log.h"

namespace GIF
{
void process_packet(u8* mem, u32 addr)
{
    gif_tag_t tag = *reinterpret_cast<gif_tag_t*>(&mem[addr]);
    log_print("GIF", "cmd: " + to_string(tag.cmd), log_level::warning);
}

void receive_dma(u32 addr)
{
    u8* mem = nullptr;

    if(addr & 0x80000000)
    {
        mem = (u8*)MemoryEE::spr;
        addr &= 0x3fff;
    }
    else
    {
        mem = (u8*)MemoryEE::ram;
        addr &= 0x1ffffff;
    }

    process_packet(mem, addr);
}
}
