#pragma once

#include "common/common.h"
#include "core/memmap.h"

namespace ELF
{
struct ELF_prog_hdr
{
    u32 type;
    u32 offset;
    u32 vaddr;
    u32 paddr;
    u32 file_size;
    u32 mem_size;
    u32 flags;
    u32 alignment;
};

struct ELF_header
{
    u8 id[16];
    u16 type;
    u16 cpu;
    u32 version;
    u32 entry_point;
    u32 prog_hdr_start;
    u32 sect_hdr_start;
    u32 flags;
    u16 size;
    u16 prog_hdr_entry_size;
    u16 prog_hdr_count;
    u16 sect_hdr_entry_size;
    u16 sect_hdr_count;
    u16 sect_hdr_str_tbl_index;
};

struct ELF
{
    ELF_header hdr;
    std::vector<ELF_prog_hdr> prog_hdrs;

    void load(FILE* fp);
};
}
