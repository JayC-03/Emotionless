#include "common/memory_util.h"
 
void* alloc_exec_mem(int size)
{
  void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);
  return ptr;
}

void free_mem_pages(void* ptr, int size)
{
  if(ptr) munmap(ptr,size);
}