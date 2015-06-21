//Derived from Dolphin code

#include <cstring>

#include "common/common_types.h"
#include "common/cpu_detect.h"

static inline void cpuid(int info[4], int func_id, int subfunc_id)
{
    info[0] = func_id;
    info[2] = subfunc_id;

    __asm__("cpuid"
            : "=a" (info[0]),
            "=b" (info[1]),
            "=c" (info[2]),
            "=d" (info[3])
            : "a" (func_id),
            "c" (subfunc_id));
}

static u64 xgetbv(u32 index)
{
    u32 eax, edx;
    __asm__("xgetbv"
            : "=a" (eax),
            "=d" (edx)
            :
            "c" (index));
    return ((u64)edx << 32) | eax;
}

void CPUInfo::Detect()
{
    memset(this, 0, sizeof(*this));

    phys_cpus = 1;
    log_cpus = 1;

    int cpu_id[4];
    memset(brand_string, 0, sizeof(brand_string));

    cpuid(cpu_id, 0x00000000, 0);
    u32 max_std_fn = cpu_id[0];
    *((int*)brand_string) = cpu_id[1];
    *((int*)(brand_string + 4)) = cpu_id[3];
    *((int*)(brand_string + 8)) = cpu_id[2];
    cpuid(cpu_id, 0x80000000, 0);
    u32 max_ext_fn = cpu_id[0];

    if(!strcmp(brand_string, "GenuineIntel")) vendor = VENDOR_INTEL;
    else if(!strcmp(brand_string, "AuthenticAMD")) vendor = VENDOR_AMD;
    else vendor = VENDOR_OTHER;

    if(max_std_fn >= 1)
    {
        cpuid(cpu_id, 0x00000001, 0);
        int family = ((cpu_id[0] >> 8) & 0xf) + ((cpu_id[0] >> 20) & 0xff);
        int model = ((cpu_id[0] >> 4) & 0xf) + ((cpu_id[0] >> 12) & 0xf0);

        log_cpus = (cpu_id[1] >> 16) & 0xFF;

        if((cpu_id[2])       & 1) SSE3 = true;
        if((cpu_id[2] >> 9)  & 1) SSSE3 = true;
        if((cpu_id[2] >> 19) & 1) SSE4_1 = true;
        if((cpu_id[2] >> 20) & 1) SSE4_2 = true;

        if(((cpu_id[2] >> 28) & 1) && ((cpu_id[2] >> 27) & 1))
        {
            if((xgetbv(0) & 0x6) == 0x6)
            {
                AVX = true;
                if((cpu_id[2] >> 12) & 1) FMA3 = true;
            }
        }

        if(max_std_fn >= 7)
        {
            cpuid(cpu_id, 0x00000007, 0);
            if((cpu_id[1] >> 5)  & 1) AVX2 = AVX;
            if((cpu_id[1] >> 16) & 1) AVX512F = AVX2;
        }
    }

    if(max_ext_fn >= 0x80000004)
    {
        cpuid(cpu_id, 0x80000002, 0);
        memcpy(cpu_string, cpu_id, sizeof(cpu_id));
        cpuid(cpu_id, 0x80000003, 0);
        memcpy(cpu_string + 16, cpu_id, sizeof(cpu_id));
        cpuid(cpu_id, 0x80000004, 0);
        memcpy(cpu_string + 32, cpu_id, sizeof(cpu_id));
    }
}
