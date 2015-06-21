// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

void* alloc_exec_mem(int size);
void free_mem_pages(void* ptr, int size);
