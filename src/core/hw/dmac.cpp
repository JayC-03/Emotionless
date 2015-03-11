#include "core/hw/dmac.h"

namespace DMAC
{
void Write32(u32 addr, u32 data)
{
    //TODO: Implement
    //NOTE: For some reason, the first PS2 BIOS tries to write to 0x1000F500, which isn't a documented register. hwtests are welcomed here.
}
}