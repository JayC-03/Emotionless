// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "common/common.h"
#include "common/memory_util.h"

template<class T> class code_block : public T
{
private:
    virtual void poison() = 0;
protected:
    u8* region;
    int region_size;
public:
    code_block() : region(nullptr), region_size(0) {}
    virtual ~code_block() {
        if(region) free_code_space();
    }

    void alloc_code_space(int size)
    {
        region_size = size;
        region = (u8*)alloc_exec_mem(region_size);
        T::set_code_ptr(region);
    }
    void clear_code_space()
    {
        poison();
        T::set_code_ptr(region);
    }
    void free_code_space()
    {
        free_mem_pages(region,region_size);
        region = nullptr;
        region_size = 0;
    }
};
