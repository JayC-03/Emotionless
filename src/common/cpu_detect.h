// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

enum cpu_vendor
{
    VENDOR_INTEL,
    VENDOR_AMD,
    VENDOR_ARM,
    VENDOR_OTHER
};

struct CPUInfo
{
    cpu_vendor vendor;

    char cpu_string[0x41];
    char brand_string[0x21];

    int phys_cpus;
    int log_cpus;

    //x86-specific features
    //Since we only support x86-64, SSE and SSE2 are always used, as 64-bit x86 implies them
    bool SSE3;
    bool SSSE3;
    bool POPCNT;
    bool SSE4_1;
    bool SSE4_2;
    bool LZCNT;
    bool SSE4a;
    bool AVX;
    bool AVX2;
    bool FMA3;
    bool FMA4;
    bool AVX512F;

    bool LAHFSAHF64;

    //ARMv8-specific features
    bool ARMv8_1;

    void Detect();
};

extern CPUInfo cpu_info;
