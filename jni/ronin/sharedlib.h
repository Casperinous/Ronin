#ifndef SHAREDLIB_H_
#define SHAREDLIB_H_

#include <elf.h>
#include <stdbool.h>
#include "mapsharedlib.h"
#include "melf.h"
#include "hash.h"

/*
 * Temporary, in order to see where 
 * we will gonna need them. Probably, 
 * they will be merge with the extra 
 * properties be located inside shared_lib
 * struct
 */


typedef struct {

	unsigned long vbase;
	unsigned long vend;
	
	ElfW(Addr) load_bias;

	map_shared_lib **shared_lib_segments;
	int shared_lib_segments_num;


	/*
	 * Pointer to current elf header
	 */
	ElfW(Ehdr) *eh;

	/* 
	 * Pointer to current elf program header
	 */

	ElfW(Phdr) *ph;

	/*
	 * Pointer to current Dynamic Section ( plz god not null keke )
	 */

	ElfW(Dyn) *dyn;

	/*
	 * Pointer to current elf sections
	 */

	ElfW(Shdr) *sh;

	/*
	 * Pointer to symbol table 
	 */
	ElfW(Sym) *sym;

	/*
	 * Must include the defines for
	 * bool definitions later on. For 
	 * now use this temporary.
	 */
	bool has_gnu_hash;

	/*
	 * Pointer to hashes structures
	 */

	dt_hash *dhash;

	dt_gnu_hash *dghash;

	const char * strtab;

	ElfW(Rel) * rel;
	
	size_t rel_count;

	/*
	 * Pointer to rela table + size of rela table
	 */
	ElfW(Rela) * rela;

	size_t rela_count;

	/*
	 * Point to got.rela table + size of got.rela table
	 */
	ElfW(Rela) * plt_rela;

	size_t  plt_rela_count;

	ElfW(Rel) * plt_rel;

	size_t plt_rel_count;

	/*
	 * Points to got table holding data from .got.plt
	 */
	unsigned long * got;

} shared_lib;



static shared_lib *
init_sh_lib();

shared_lib *
find_shared_lib(char *libname);

int 
extr_dyn_sec_info(shared_lib * lib);

void 
init_shared_lib_addr_space(shared_lib * lib);

void 
insert_shared_lib_segment(shared_lib * lib, map_shared_lib * segment);


static void 
init_shared_lib_arr (shared_lib *library);


map_shared_lib **
expand_map_shared_lib_arr(map_shared_lib ** lib, int size);

static void 
set_load_bias(shared_lib *lib);

void free_shared_lib(shared_lib * lib);

static void free_internal_strs(shared_lib * lib);


#endif 
