#include "core/hw/dmac.h"
#include "core/hw/gif.h"
#include "core/memmap.h"
#include "core/ee/ee.h"
#include "common/log.h"

namespace DMAC
{
dmac_channel_t dmac_channels[10];

u32 dmac_status;

void dmac_channel_t::single_step()
{
    if(chcr & 0x100) //Check STR
    {
        switch((chcr >> 2) & 0x3) //Check MOD
        {
        case 1:
        {
            //Chain mode
            single_step_chain();
            break;
        }
        }
    }
}

void dmac_channel_t::run()
{
    if(chcr & 0x100) //Check STR
    {
        switch((chcr >> 2) & 0x3) //Check MOD
        {
        case 1:
        {
            //Chain mode
            while(chcr & 0x100)
            {
                single_step_chain();
            }
            break;
        }
        }
    }
}

void dmac_channel_t::single_step_chain()
{
    if(qwc == 0)
    {
        u8 tag = (chcr >> 28) & 7;
        if(tag == 0 || tag == 7)
        {
            chcr &= 0xfffffeff;
            log_print("DMAC", log_level::warning, "DMA finished");
        }
    }

    u64 tag = fetch_dma_tag(tadr);

    chcr &= 0xffff;
    chcr |= tag & 0xffff0000;

    u8 id = (tag >> 28) & 0x07;

    switch(id)
    {
    case 0:
    {
        madr = tag >> 32;
        qwc = tag & 0xffff;
        tadr += 0x10;
        break;
    }
    case 1:
    {
        madr = tadr + 0x10;
        qwc = tag & 0xffff;
        tadr = (qwc << 4) + madr;
        break;
    }
    case 2:
    {
        madr = tadr + 0x10;
        qwc = tag & 0xffff;
        tadr = tag >> 32;
        break;
    }
    case 3:
    case 4:
    {
        madr = tag >> 32;
        qwc = tag & 0xffff;
        tadr += 0x10;
        break;
    }
    case 5:
    {
        madr = tadr + 0x10;
        qwc = tag & 0xffff;
        asr[(chcr >> 4) & 3] = madr + (qwc << 4);
        tadr = tag >> 32;

        //Evil evil bit manipulation. Essentially, this is just CHCR.ASP++
        u8 asp = (chcr >> 4) & 3;
        chcr &= 0xffffffcf;
        asp++;
        asp &= 3;
        chcr |= asp << 4;
        break;
    }
    case 6:
    {
        madr = tadr + 0x10;
        qwc = tag & 0xffff;
        if(chcr & 0x30)
        {
            u8 asp = (chcr >> 4) & 3;
            chcr &= 0xffffffcf;
            asp--;
            asp &= 3;
            chcr |= asp << 4;
            tadr = asr[asp];
        }
        break;
    }
    case 7:
    {
        madr = tadr + 0x10;
        qwc = tag & 0xffff;
        break;
    }
    }

    if(qwc != 0)
    {
        receive(madr);
        madr++;
        qwc--;
    }
}

u64 fetch_dma_tag(u32 addr)
{
    if(addr & 0x80000000)
    {
        return *(u64*)&MemoryEE::spr[addr & 0x3fff];
    }
    else return *(u64*)&MemoryEE::ram[addr & 0x1ffffff];
}

void unknown_receiver(u32 addr)
{
    log_print("DMAC", log_level::warning, "Unknown DMA receiver!");
}

void init_dmac_channels()
{
    for(int i = 0; i < 10; i++)
    {
        dmac_channels[i].receive = unknown_receiver;
    }

    dmac_channels[2].receive = GIF::receive_dma;
}

u32 Read32(u32 addr)
{
    u32 res = 0;
    switch(addr)
    {
    case 0x1000a000:
    {
        res = dmac_channels[2].chcr;
        break;
    }
    case 0x1000a020:
    {
        res = dmac_channels[2].qwc;
        break;
    }
    case 0x1000a030:
    {
        res = dmac_channels[2].tadr;
        break;
    }
    case 0x1000c000:
    {
        res = dmac_channels[5].chcr;
        break;
    }
    case 0x1000e010:
    {
        res = dmac_status;
        break;
    }
    default:
    {
        log_print("DMAC", log_level::warning, "Read32 from unimplemented DMAC register at %08x!", addr);
        break;
    }
    }
    return res;
}

void Write32(u32 addr, u32 data)
{
    //NOTE: For some reason, the first PS2 BIOS tries to write to 0x1000F500, which isn't a documented register. hwtests are welcomed here.
    switch(addr)
    {
    case 0x1000a000:
    {
        dmac_channels[2].chcr = data;
        EE::interpreter->end_block = true;
        break;
    }
    case 0x1000a010:
    {
        dmac_channels[2].madr = data;
        break;
    }
    case 0x1000a020:
    {
        dmac_channels[2].qwc = data;
        break;
    }
    case 0x1000a030:
    {
        dmac_channels[2].tadr = data;
        break;
    }
    case 0x1000a040:
    {
        dmac_channels[2].asr[0] = data;
        break;
    }
    case 0x1000a050:
    {
        dmac_channels[2].asr[1] = data;
        break;
    }
    case 0x1000a080:
    {
        dmac_channels[2].sadr = data;
        break;
    }
    case 0x1000e010:
    {
        u32 stat = data & 0x0000ffff;
        u32 mask = data & 0xffff0000;
        dmac_status ^= mask;
        dmac_status &= ~stat;
        break;
    }
    default:
    {
        log_print("DMAC", log_level::warning, "Write32 to unimplemented DMAC register %08x with %08x!", addr, data);
        break;
    }
    }
}

void single_step()
{
    for(int i = 0; i<10; i++)
    {
        dmac_channels[i].single_step();
    }
}

void run()
{
    for(int i = 0; i<10; i++)
    {
        dmac_channels[i].run();
    }
}
}
