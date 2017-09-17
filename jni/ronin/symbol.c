#include "symbol.h"
#include "hash.h"
#include "sharedlib.h"
#include "melf.h"
#include "log.h"
#include <stdlib.h>
#include <elf.h>
#include <string.h>

Symbol * 
init_sym_str(const char * name, bool has_gnu_hash)
{
	Symbol * sym = (Symbol *)malloc(sizeof(Symbol));
	if( sym )
	{
		sym-> name = name;
		sym->has_gnu_hash = has_gnu_hash;
		sym->reloc = 0;
		sym->type = 0;
	}

	return sym;

}



Symbol * do_sym_lookup(shared_lib * lib, const char * sym_name)
{
	if( lib == NULL || sym_name == NULL ) return NULL;
	return do_internal_lookup(lib,sym_name);
}

static Symbol * do_internal_lookup(shared_lib * lib, const char * name)
{
	/*
	 * Here should be the bloom filter procedure but for now
	 * we will perform a linear search
	 */

	Symbol * sym =  init_sym_str(name, lib->has_gnu_hash);

	if( sym )
	{

#if defined(USE_RELA)
		ElfW(Rela) * rela = get_rela_str(lib,name);
		if( rela )
		{
			sym->reloc = (unsigned long)(rela->r_offset + lib->vbase);
			sym->type = ELFW(R_TYPE)(rela->r_info);
		}			
#else 

		ElfW(Rel) * rel = get_rel_str(lib, name);
		if( rel )
		{
			sym->reloc = (unsigned long)(rel->r_offset + lib->vbase);
			sym->type = ELFW(R_TYPE)(rel->r_info);
		}	
#endif
		
		
		if( sym->has_gnu_hash )
			sym->gnu_hash_ = calc_gnu_hash( sym->name );
		else
			sym->elf_hash_ = calc_elf_hash( sym->name );	
	}

	return sym;

}

static ElfW(Rela) * get_rela_str(shared_lib * lib, const char * name)
{
	ElfW(Rela) * rela = lib->plt_rela;
	for(size_t i = 0; i < lib->plt_rela_count; i++, ++rela)
	{
		ElfW(Word) sym_ = ELFW(R_SYM)(rela->r_info);
		ElfW(Sym) * sym = (ElfW(Sym) *)(lib->sym + sym_);
		char * sym_name = (char *)(lib->strtab + sym->st_name);
		if( strncmp(sym_name, name, strlen(sym_name)) == 0)
		{
			LOGX(INFO,"[+] Found requested symbol %s", sym_name);
			return rela;

		}
	}

	return NULL;
}

static ElfW(Rel) * get_rel_str(shared_lib * lib, const char * name)
{
	ElfW(Rel) * rel = lib->plt_rel;
	for(size_t i = 0; i < lib->plt_rel_count; i++, ++rel)
	{
		ElfW(Word) sym_ = ELFW(R_SYM)(rel->r_info);
		ElfW(Sym) * sym = (ElfW(Sym) *)(lib->sym + sym_);
		char * sym_name = (char *)(lib->strtab + sym->st_name);
		if( strncmp(sym_name, name, strlen(sym_name)) == 0)
		{
			LOGX(INFO,"[+] Found requested symbol %s", sym_name);
			return rel;

		}
	}

	return NULL;
}


