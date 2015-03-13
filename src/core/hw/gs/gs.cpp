#include "core/hw/gs/gs.h"

#include "common/log.h"

namespace GS
{
gs_state_t gs_state;

void Write32(u32 addr, u32 data)
{
    if((addr >> 16) == 0x1200)
    {
        //Privileged registers
        switch(addr & 0xffff)
        {
        case 0x0070:
        case 0x0074:
        {
            if(addr & 4)
            {
                gs_state.dispfb1.actual = gs_state.dispfb1.latched | ((u64)data << 32);
                gs_state.dispfb1.latched = 0;
            }
            else
            {
                gs_state.dispfb1.latched = data;
            }
            break;
        }
        case 0x0080:
        case 0x0084:
        {
            if(addr & 4)
            {
                gs_state.display1.actual = gs_state.display1.latched | ((u64)data << 32);
                gs_state.display1.latched = 0;
            }
            else
            {
                gs_state.display1.latched = data;
            }
            break;
        }
        case 0x0090:
        case 0x0094:
        {
            if(addr & 4)
            {
                gs_state.dispfb2.actual = gs_state.dispfb2.latched | ((u64)data << 32);
                gs_state.dispfb2.latched = 0;
            }
            else
            {
                gs_state.dispfb2.latched = data;
            }
            break;
        }
        case 0x00a0:
        case 0x00a4:
        {
            if(addr & 4)
            {
                gs_state.display2.actual = gs_state.display2.latched | ((u64)data << 32);
                gs_state.display2.latched = 0;
            }
            else
            {
                gs_state.display2.latched = data;
            }
            break;
        }
        default:
        {
            log_print("GS", "Write32 to unimplemented GS register!", log_level::warning);
            break;
        }
        }
    }
}
}
