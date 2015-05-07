#pragma once

#include "common/common.h"

namespace HLE
{
enum SYSCALL_REGS
{
    RETURN = 2,
    PARAM0 = 4,
    PARAM1 = 5,
    PARAM2 = 6,
    PARAM3 = 7,
    PARAM4 = 8,
    PARAM5 = 9,
};

enum THREAD_STATUS
{
    THREAD_RUNNING = 1,
    THREAD_ZOMBIE  = 7,
};

struct thread_param_t
{
    u32 status;
    u32 thread_proc;
    u32 stack_base;
    u32 stack_size;
    u32 gp;
    u32 priority;
    u32 current_priority;
};

struct thread_t
{
    u32 valid;
    u32 status;
    u32 ctx_ptr;
    u32 stack_base;
    u32 heap_base;
    u32 thread_proc;
    u32 epc;
    u32 priority;
    u32 sema_wait;
    u32 wake_up_count;
    u32 sched_id;
    u32 stack_size;
    u32 quota;
};

struct semaphore_param_t
{
    u32 count;
    u32 max_count;
    u32 init_count;
    u32 wait_threads;
    u32 attributes;
    u32 options;
};

struct semaphore_t
{
    u32 valid;
    u32 count;
    u32 max_count;
    u32 wait_count;
};

struct intc_handler_t
{
    u32 valid;
    u32 cause;
    u32 addr;
    u32 arg;
    u32 gp;
};

struct dmac_handler_t
{
    u32 valid;
    u32 channel;
    u32 addr;
    u32 arg;
    u32 gp;
};

extern intc_handler_t intc_handler;

extern dmac_handler_t dmac_handler;

extern std::function<void(u8)> syscalls[128];

void init_syscalls();

void unknown_syscall(u8 syscall);

void custom_syscall(u8 syscall);

void add_intc_handler(u8 syscall);
void add_dmac_handler(u8 syscall);
void create_thread(u8 syscall);
void start_thread(u8 syscall);
void get_thread_id(u8 syscall);
void init_main_thread(u8 syscall);
void init_heap(u8 syscall);
void heap_end(u8 syscall);
void create_sema(u8 syscall);
void set_syscall(u8 syscall);
}
