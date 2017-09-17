#include <stdlib.h>
#include "elfutils.h"
#include "memutils.h"
#include "log.h"
#include "hash.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>

	
	
ElfW(Ehdr) *
elf_get_header_ptr(unsigned long base, unsigned long end)
{
	/*
	 * There is a huge discussion and error with this approach. 
	 * Judging from the source code of tools like libelf etc etc, 
	 * it seems that some times due to memory aligment we can not
	 * access the memory directly ( by deferencing ). For now, 
	 * because in aarch64 it works, we will approach it that way.
	 */
	ElfW(Ehdr) * eh = (ElfW(Ehdr) *)(base);
	while(strncmp((char *)eh->e_ident, ELF_MAG, strlen(ELF_MAG)) && base < end )
	{
		ElfW(Ehdr) * eh = (ElfW(Ehdr) *)(++base);

	}

	int elf_class = eh->e_ident[EI_CLASS];

	if( elf_class == ELFCLASS64 )
		LOGX(INFO,"%s","[+] Architecture is 64 bit \n");

	return strncmp((char *)eh->e_ident, ELF_MAG, strlen(ELF_MAG)) == 0 ? eh : NULL;
}



ElfW(Dyn) *
elf_get_dyn_ptr(shared_lib *lib)
{
	ElfW(Dyn) * dyn = NULL;
	if( lib == NULL || lib->eh == NULL ) return dyn;
	void * str = (void *)(lib->vbase + lib->eh->e_phoff + lib->eh->e_phentsize);
	ElfW(Phdr) *phdr = (ElfW(Phdr) *)str; 
	for( uint16_t i = 0; i < lib->eh->e_phnum; i++)
	{
		if( phdr->p_type == PT_DYNAMIC )
		{
			dyn = (ElfW(Dyn) *)(lib->vbase + phdr->p_vaddr);
			LOGX(INFO,"[+] Found PT_DYNAMIC at address %p \n", dyn);
			LOGX(INFO,"[+] Dynamic section from %p to %p \n", dyn, (char *)dyn + phdr->p_memsz);
			lib->got = (unsigned long *)((char *)dyn + phdr->p_memsz);
			break;
		}
		phdr++;
	}	
	
	return dyn;

}

int 
extr_dyn_sec_info(shared_lib *lib)
{
	
	int count = 0;
	ElfW(Dyn) *dyn_ = lib->dyn; 
	for(; dyn_->d_tag != DT_NULL; ++dyn_)
	{
		switch(dyn_->d_tag)
		{
			case DT_PLTGOT:
				LOGX(INFO,"[+] Found DT_PLTGOT at address %p \n", dyn_);
				count;
				break;
			case DT_HASH:
				LOGX(INFO,"[+] Found DT_HASH at address %p \n", dyn_);
				lib->dhash = init_dt_hash(lib->load_bias,(uint32_t )(dyn_->d_un.d_ptr));
				lib->has_gnu_hash = false;
				count++;
				break;
			case DT_GNU_HASH:
				LOGX(INFO,"[+] Found DT_GNU_HASH at address %p \n", dyn_);
				lib->dghash = init_dt_gnu_hash(lib->load_bias, (uint32_t )(dyn_->d_un.d_ptr));
				lib->has_gnu_hash = true;
				count++;
				break;
			case DT_SYMTAB:
				LOGX(INFO,"[+] Found DT_SYMTAB at address %p \n", dyn_);
				lib->sym = (ElfW(Sym) *)(lib->vbase + dyn_->d_un.d_ptr);
				count++;
				break;
			case DT_STRTAB:
				LOGX(INFO,"[+] Found DT_STRTAB at address %p \n", dyn_);
				lib->strtab = (const char *)(lib->vbase + dyn_->d_un.d_ptr);
				count++;
				break;
			case DT_JMPREL:
				LOGX(INFO,"[+] Found DT_JMPREL at address %p \n", dyn_);
#if defined(USE_RELA)

				lib->plt_rela = (ElfW(Rela) *)(lib->vbase + dyn_->d_un.d_ptr);
#else
				lib->plt_rel = (ElfW(Rel) *)(lib->vbase + dyn_->d_un.d_ptr);
#endif
				count++;
				break;
			case DT_PLTRELSZ:
#if defined(USE_RELA)
				LOGX(INFO,"[+] Found DT_PTLRELSZ at address %p \n", dyn_);
				lib->plt_rela_count = dyn_->d_un.d_val / sizeof(ElfW(Rela));
#else
				lib->plt_rel_count =  dyn_->d_un.d_val / sizeof(ElfW(Rel));
#endif
				break;
			case DT_REL:
				LOGX(INFO,"[+] Found DT_REL at address %p \n", dyn_);
				lib->rel = (ElfW(Rel) *)(lib->load_bias + dyn_->d_un.d_ptr);
				break;
			case DT_RELSZ:
				LOGX(INFO,"[+] Found DT_RELSZ at address %p \n", dyn_);	
				lib->rel_count = dyn_->d_un.d_val / sizeof(ElfW(Rel));
				break;
			case DT_RELA:
				LOGX(INFO,"[+] Found DT_RELA at address %p \n", dyn_);
				lib->rela = (ElfW(Rela) *)(lib->load_bias + dyn_->d_un.d_ptr);
				break;
			case DT_RELASZ:
				LOGX(INFO,"[+] Found DT_RELASZ at address %p \n", dyn_);	
				lib->rela_count = dyn_->d_un.d_val / sizeof(ElfW(Rela));
				break;
				

		}
	}
	for(ElfW(Dyn) *dyn = lib->dyn; dyn->d_tag != DT_NULL; ++dyn)
	{
		switch(dyn->d_tag)
		{
			case DT_SONAME:
				LOGX(INFO, "[+] SONAME ---> %s \n", lib->strtab + dyn->d_un.d_val);
				break;
			case DT_RUNPATH:
				LOGX(INFO, "[+] RUNPATH ---> %s \n", lib->strtab + dyn->d_un.d_val);
				break;
		}
	}

	
	return count;
}


void 
elf_print_sym(shared_lib * lib)
{
	if ( lib == NULL ) return;


#if defined(USE_RELA)
	if( lib->plt_rela != NULL && lib->plt_rela_count != 0)
	{
		
		LOGX(INFO,"%s","[*] Printing symbols in rela.plt table.");
		ElfW(Rela) * rela = lib->plt_rela;
		
		for(size_t i = 0; i < lib->plt_rela_count; i++, ++rela)
		{
	
		 	ElfW(Word) sym_ = ELFW(R_SYM)(rela->r_info);
		 	ElfW(Sym) * sym = (ElfW(Sym) *)(lib->sym + sym_);
		 	
			ElfW(Word) type = ELFW(R_TYPE)(rela->r_info);
			ElfW(Addr) offset = rela->r_offset;
			
			char * sym_name = (char *)(lib->strtab + sym->st_name);
			LOGX(INFO,"[%lu] Symbol with name %s has type 0x%llx and offset 0x%llx",
					i,
					sym_name,
					type,
					offset);


						
		}

		
	}
#else

	if( lib->plt_rel != NULL && lib->plt_rel_count != 0)
	{
		
		LOGX(INFO,"%s","[*] Printing symbols in rel.plt table.");
		ElfW(Rel) * rel = lib->plt_rel;
		
		for(size_t i = 0; i < lib->plt_rel_count; i++, ++rel)
		{
	
			ElfW(Word) sym_ = ELFW(R_SYM)(rel->r_info);
			ElfW(Sym) * sym = (ElfW(Sym) *)(lib->sym + sym_);
			
			ElfW(Word) type = ELFW(R_TYPE)(rel->r_info);
			ElfW(Addr) offset = rel->r_offset;
			
			char * sym_name = (char *)(lib->strtab + sym->st_name);
			LOGX(INFO,"[%lu] Symbol with name %s has type 0x%llx and offset 0x%llx",
					i,
					sym_name,
					type,
					offset);
						
		}

	}

#endif	


}


