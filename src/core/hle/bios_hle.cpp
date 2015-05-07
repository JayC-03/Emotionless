#include "core/hle/bios_hle.h"
#include "core/ee/ee.h"

#include "common/log.h"

namespace HLE
{
std::function<void(u8)> syscalls[128];

std::vector<thread_t> threads;
std::vector<semaphore_t> semaphores;

intc_handler_t intc_handler;

dmac_handler_t dmac_handler;

u32 current_thread_id;

struct syscall_t
{
    u8 syscall;
    std::function<void(u8)> func;
};

static syscall_t primary_table[] =
{
    {0x10, add_intc_handler},
    {0x12, add_dmac_handler},
    {0x20, create_thread},
    {0x22, start_thread},
    {0x2f, get_thread_id},
    {0x3c, init_main_thread},
    {0x3d, init_heap},
    {0x3e, heap_end},
    {0x40, create_sema},
    {0x74, set_syscall},
};

void unknown_syscall(u8 syscall)
{
    log_print("HLE", "Unknown syscall!", log_level::warning);
}

std::array<u32,0x80> custom_syscall_addrs;

//Clever hack to allow using a table of function pointers.
void custom_syscall(u8 syscall)
{
    PC = custom_syscall_addrs[syscall & 0x7f];
}

void init_syscalls()
{
    for(int i = 0; i < 128; i++)
    {
        syscalls[i] = unknown_syscall;
    }

    for(auto& syscall : primary_table)
    {
        syscalls[syscall.syscall] = syscall.func;
    }
}

void add_intc_handler(u8 syscall)
{
    u32 cause = rGPR[PARAM0].ul[0];
    u32 addr = rGPR[PARAM1].ul[0];
    u32 arg = rGPR[PARAM3].ul[0];

    intc_handler.valid = 1;
    intc_handler.addr = addr;
    intc_handler.cause = cause;
    intc_handler.arg = arg;
    intc_handler.gp = rGPR[28].ul[0];

    rGPR[RETURN].ud[0] = 1;
    rGPR[RETURN].ud[1] = 0;
}

void add_dmac_handler(u8 syscall)
{
    u32 channel = rGPR[PARAM0].ul[0];
    u32 addr = rGPR[PARAM1].ul[0];
    u32 arg = rGPR[PARAM3].ul[0];

    dmac_handler.valid = 1;
    dmac_handler.addr = addr;
    dmac_handler.channel = channel;
    dmac_handler.arg = arg;
    dmac_handler.gp = rGPR[28].ul[0];

    rGPR[RETURN].ud[0] = 1;
    rGPR[RETURN].ud[1] = 0;
}

void create_thread(u8 syscall)
{
    u32 heap_base = threads[current_thread_id].heap_base;
    u32 id = threads.size();

    thread_t thread;
    thread.valid = 1;
    thread.status = THREAD_ZOMBIE;
    thread.stack_base = rGPR[PARAM2].ul[0];
    thread.epc = rGPR[PARAM1].ul[0];
    thread.thread_proc = rGPR[PARAM1].ul[0];
    thread.priority = rGPR[PARAM5].ul[0];
    thread.heap_base = heap_base;
    thread.wake_up_count = 0;
    thread.stack_size = rGPR[PARAM3].ul[0];

    u32 stack_addr = thread.stack_base + thread.stack_size - 0x2a0;
    thread.ctx_ptr = stack_addr;

    EE::Write128((stack_addr + (28 << 16)), rGPR[PARAM4]);
    EE::ee_reg sp;
    sp.ud[0] = stack_addr;
    sp.ud[1] = 0;
    EE::Write128((stack_addr + (29 << 16)), sp);
    EE::Write128((stack_addr + (30 << 16)), sp);
    EE::ee_reg ra;
    ra.ud[0] = 0x1fc03000;
    ra.ud[1] = 0;
    EE::Write128((stack_addr + (31 << 16)), ra);

    threads.push_back(thread);
    rGPR[RETURN].ud[0] = id;
}

void start_thread(u8 syscall)
{
    u32 id  = rGPR[PARAM0].ul[0];
    u32 arg = rGPR[PARAM1].ul[0];

    if(!threads.at(id).valid)
    {
        rGPR[RETURN].ul[0] = 0xffffffff;
        rGPR[RETURN].ul[1] = 0xffffffff;
        return;
    }

    threads.at(id).status = THREAD_RUNNING;
    threads.at(id).epc = threads.at(id).thread_proc;

    EE::ee_reg a0;
    a0.ud[0] = arg;
    a0.ud[1] = 0;
    EE::Write128((threads.at(id).ctx_ptr + (3 << 16)), a0);

    EE::ee_reg sp;
    sp.ud[0] = threads.at(id).ctx_ptr;
    sp.ud[1] = 0;
    EE::Write128((threads.at(id).ctx_ptr + (29 << 16)), sp);
    EE::Write128((threads.at(id).ctx_ptr + (30 << 16)), sp);

    EE::ee_reg ra;
    ra.ud[0] = 0x1fc03000;
    ra.ud[1] = 0;
    rGPR[RETURN].ud[0] = current_thread_id;
}

void get_thread_id(u8 syscall)
{
    rGPR[RETURN].ud[0] = current_thread_id;
}

void init_main_thread(u8 syscall)
{
    log_print("HLE", "Initializing main thread!", log_level::warning);

    u32 stack_base = rGPR[PARAM1].ul[0];
    u32 stack_size = rGPR[PARAM2].ul[0];

    u32 stack_addr;
    if(stack_base == 0xffffffff) stack_addr = 0x0200000;
    else stack_addr = stack_base + stack_size;

    thread_t thread;
    thread.valid = 1;
    thread.status = THREAD_RUNNING;
    thread.stack_base = stack_addr - stack_size;
    thread.priority = 0;

    stack_addr -= 0x2a0;
    thread.ctx_ptr = stack_addr;

    threads.push_back(thread);

    rGPR[RETURN].ud[0] = stack_addr;
    rGPR[RETURN].ud[1] = 0;
}

void init_heap(u8 syscall)
{
    log_print("HLE", "Initializing heap!", log_level::warning);

    u32 heap_base = rGPR[PARAM1].ul[0];
    u32 heap_size = rGPR[PARAM2].ul[0];

    if(heap_size == 0xffffffff) threads[current_thread_id].heap_base = threads[current_thread_id].stack_base;
    else threads[current_thread_id].heap_base = heap_base + heap_size;

    rGPR[RETURN].ud[0] = threads[current_thread_id].heap_base;
    rGPR[RETURN].ud[1] = 0;
}

void heap_end(u8 syscall)
{
    rGPR[RETURN].ud[0] = threads[current_thread_id].heap_base;
    rGPR[RETURN].ud[1] = 0;
}

void create_sema(u8 syscall)
{
    semaphore_t sema;
    sema.count = rGPR[PARAM2].ul[0];
    sema.max_count = rGPR[PARAM1].ul[0];
    sema.wait_count = 0;

    semaphores.push_back(sema);
    rGPR[RETURN].ud[0] = semaphores.size() - 1;
}

void set_syscall(u8 syscall)
{
    u32 num = rGPR[PARAM0].ul[0];
    u32 addr = rGPR[PARAM1].ul[0];

    syscalls[num & 0x7F] = custom_syscall;
    custom_syscall_addrs[num & 0x7F] = addr;
}

}
