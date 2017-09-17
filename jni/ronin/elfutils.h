#ifndef ELFUTILS_H_
#define ELFUTILS_H_


#include <elf.h>
#include "sharedlib.h"
#include "mapsharedlib.h"
#include "melf.h"


static const char ELF_MAG[] = {ELFMAG0,ELFMAG1,ELFMAG2,ELFMAG3, 0x00};

ElfW(Ehdr)*
elf_get_header_ptr(unsigned long base, unsigned long end);

ElfW(Dyn) *
elf_get_dyn_ptr(shared_lib *lib);

int 
extr_dyn_sec_info(shared_lib *lib);


void 
elf_print_sym(shared_lib * lib);

#endif 
