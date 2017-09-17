#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdlib.h>
#include <stdbool.h>
#include "sharedlib.h"
#include "melf.h"

typedef struct {
	
	/*
	 * Symbol's name calculated based on
	 * strtab + idx
	 */
	const char * name;

	ElfW(Word) type;

	unsigned long reloc;

	/*
	 * Need in order to define the existence
	 * of a symbol
	 */
	uint32_t elf_hash_;
	
	uint32_t gnu_hash_;

	/*
	 * Again, why GNU why
	 */
	bool has_gnu_hash;

	/*
	 * Should also include information regarding
	 * the memory address of the symbol (ElfAddr)
	 * but for now, we will leave it empty
	 */

} Symbol;



Symbol * 
init_sym_str(const char * name, bool has_gnu_hash);

Symbol * 
do_sym_lookup(shared_lib * lib, const char * sym_name);


static Symbol * 
do_internal_lookup(shared_lib * lib, const char * name);

static ElfW(Rela) *
get_rela_str(shared_lib * lib, const char * name);

static ElfW(Rel) * 
get_rel_str(shared_lib * lib, const char * name);

#endif
