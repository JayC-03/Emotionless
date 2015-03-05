#include "core/hle/elf.h"
#include "core/ee/ee.h"

namespace ELF
{
void ELF::load(FILE* fp)
{
  fread(&hdr,1,sizeof(ELF_header),fp);
  
  if(hdr.id[0] != 0x7F || hdr.id[1] != 'E' || hdr.id[2] != 'L' || hdr.id[3] != 'F') return;
  
  fseek(fp,hdr.prog_hdr_start,SEEK_SET);
  
  for(int i = 0; i < hdr.prog_hdr_count; i++)
  {
    ELF_prog_hdr tmp;
    fread(&tmp, 1, sizeof(ELF_prog_hdr), fp);
    prog_hdrs.push_back(tmp);
  }
  
  for(int i = 0; i < prog_hdrs.size(); i++)
  {
    fseek(fp,prog_hdrs[i].offset,SEEK_SET);
    fread(Memory::ram + (prog_hdrs[i].paddr >> 2), 1, prog_hdrs[i].file_size, fp);
  }
  
  EE::ee_state.pc = hdr.entry_point;
}
}