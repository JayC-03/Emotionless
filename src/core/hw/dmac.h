#pragma once

#include "common/common.h"

namespace DMAC
{
struct dmac_channel_t
{
    u32 chcr;
    u32 madr;
    u32 qwc;
    u32 tadr;
    u32 asr[2];
    void single_step();
    void single_step_chain();
	void run();
    std::function<void(u32)> receive;
};

extern dmac_channel_t dmac_channels[10];

extern u32 dmac_status;

u64 fetch_dma_tag(u32 addr);

void init_dmac_channels();

u32 Read32(u32 addr);
void Write32(u32 addr, u32 data);

void single_step();
void run();
}
