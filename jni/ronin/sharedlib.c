#include <stdlib.h>
#include <limits.h>
#include <elf.h>
#include <string.h>
#include "sharedlib.h"
#include "mapsharedlib.h"
#include "elfutils.h"
#include "log.h"

static shared_lib * 
init_sh_lib()
{
	shared_lib *lib = (shared_lib *)malloc(sizeof(shared_lib));
	lib->vbase = 10000;
	lib->vend = 0;
	lib->load_bias = 0;
	init_shared_lib_arr(lib);
	lib->eh = NULL;
	lib->ph = NULL;
	lib->sh = NULL;
	lib->dyn = NULL;
	lib->sym = NULL;
	lib->dhash = NULL;
	lib->dghash = NULL;
	lib->has_gnu_hash = 0;
	lib->strtab = NULL;
	
	lib->rel = NULL;
	lib->rel_count = 0;
	lib->plt_rel = NULL;
	lib->plt_rel_count = 0;

	lib->rela = NULL;
	lib->rela_count = 0;
	lib->plt_rela = NULL;
	lib->plt_rela_count = 0;

	return lib;

}

shared_lib *find_shared_lib(char *libname)
{
	LOGX(INFO,"[*] Searching for %s", libname );
	char buffer[2048], filename[2048];
	FILE *fp = NULL;
	sprintf( filename, "/proc/%d/maps", getpid() );
	fp = fopen( filename, "rt" );
    
    	shared_lib *shared_lib = init_sh_lib();
	if( shared_lib )
	{
    		while( fgets( buffer, sizeof(buffer), fp ) ) 
    		{
    	
    			if(strstr(buffer,libname) != NULL )
			{
				map_shared_lib *lib_ = init_from_str(buffer);	
				if( lib_ )
				{
					insert_shared_lib_segment(shared_lib,lib_);
				}	
			}
    		}

    		init_shared_lib_addr_space(shared_lib);
		if( ext_lib_info(shared_lib) > 0 )
    		{
	    		LOGX(INFO, "[+] Succesfully extracted information of shared library %s", libname);
		}

    	}

	return shared_lib;

}





/*
 * Again, the uint64 is based on mine aarch64 Nexus.
 * It must be architecture agnostic.Also, there is a
 * huge mess between the sizes of various variables.
 * load bias should not be that big.
 */


map_shared_lib **
expand_map_shared_lib_arr(map_shared_lib **lib, int size)
{
	return (map_shared_lib **)realloc(lib, sizeof(map_shared_lib *) * size);
}


void 
init_shared_lib_addr_space(shared_lib * lib)
{

	if ( lib == NULL || lib->shared_lib_segments == NULL ) return;
	
	unsigned long min = ULONG_MAX, max = 0;
	int idx = 0, sg_num = lib->shared_lib_segments_num;
	for(; idx < sg_num; idx++)
	{
		map_shared_lib * tmp = (lib->shared_lib_segments[idx]);
		//printf("[*] Start -> 0x%lx End -> 0x%lx \n", tmp->base,  tmp->end);
		if( tmp->base < min )
			min = tmp->base;
		if( tmp->end > max )
			max = tmp->end;
	}

	/*
	 * Set the address space range, after
	 * looping through the mapped segments 
	 * of our shared library
	 */
	lib->vbase = min;
	lib->vend = max;

}

int 
ext_lib_info(shared_lib *lib)
{
	lib->eh = elf_get_header_ptr(lib->vbase, lib->vend);
	
	if( lib->eh == NULL ) 
	{
		LOGX(ERROR, "%s", "[-] Could not found ELF Header inside memory");
		return -1;
	}

	/* Before we call this function, we must first
	 * ensure that eh is not Null. That's why, it 
	 * is called now.
	 */
	set_load_bias(lib);	

	lib->dyn = elf_get_dyn_ptr(lib);

	if( lib->dyn == NULL ) return -1;

	if( extr_dyn_sec_info(lib) <= 0 ) return -1;

	return 1;

}

void 
insert_shared_lib_segment(shared_lib * lib, map_shared_lib * segment)
{

	
	
	/*
	 * Still, cannot decide how i will handle
	 * the error checking. Based on some public
	 * repos, die() method or some custom macro
	 * would do the job.
	 */
	if( lib == NULL || segment == NULL )
		return;

	/*
	 * Re-check, because really, in C everything
	 * and anytime can go rogue and wrong
	 */
	if( lib->shared_lib_segments == NULL)
		init_shared_lib_arr(&lib);

	if( lib->shared_lib_segments_num >= 1)
	{
		lib->shared_lib_segments_num++;
		lib->shared_lib_segments = expand_map_shared_lib_arr((lib->shared_lib_segments), lib->shared_lib_segments_num);
		if( lib->shared_lib_segments != NULL )
			lib->shared_lib_segments[lib->shared_lib_segments_num - 1] = segment;

	} else 
	{
		lib->shared_lib_segments[lib->shared_lib_segments_num] = segment;
		lib->shared_lib_segments_num++;
	}
}



static void 
init_shared_lib_arr (shared_lib *library)
{
	
	if( library != NULL )
	{
		library->shared_lib_segments = (map_shared_lib **)(malloc(sizeof(map_shared_lib *) * 1));
		library->shared_lib_segments_num = 0;

	}

}


static void 
set_load_bias(shared_lib * lib)
{
	//This gotta be somewhere else really
	void * str = (void *)(lib->vbase + lib->eh->e_phoff);
	lib->ph = (ElfW(Phdr) *)str;
	LOGX(INFO, "%s", "[*] Trying to set load_bias \n");
	//Point to the start
	ElfW(Phdr) * tmp = (ElfW(Phdr) *)str;
	for(uint16_t i = 0; i < lib->eh->e_phnum; i++)
	{
		if( tmp->p_type == PT_PHDR )
		{
			LOGX(INFO,"[+] Found PT_PHDR at address %p \n", tmp);
			lib->load_bias = lib->vbase - tmp->p_offset;
			break;
		}
		tmp++;
	}

}


void free_shared_lib(shared_lib * lib)
{
	if( lib )
	{
		free_internal_strs(lib);
		free(lib);
	}
}


static void free_internal_strs(shared_lib * lib)
{
	free_arr(lib->shared_lib_segments,lib->shared_lib_segments_num);
	free(lib->shared_lib_segments);
}


