#include "hook.h"
#include "memutils.h"
#include "symbol.h"
#include "log.h"
#include "melf.h"
#include <stdlib.h>
#include <stdbool.h>

	
hook * 
init_hook_str(char * func_name, char * exp_name, char * trg_so, char * exp_so)
{
	hook * hk = (hook *)malloc(sizeof(hook));
	if( !trg_so || !exp_so) return NULL;

	if( hk )
	{
		hk->org_addr = 0;
		hk->rpd_addr = 0;
		hk->func_name = func_name;
		hk->exp_name = exp_name;
		hk->trg_so = trg_so;
		hk->exp_so = exp_so;
		hk->ishooked = false;
	}

	return hk;
}

void hook_func(shared_lib * lib, hook * hk)
{
	Symbol * sym = 	do_sym_lookup(lib, hk->func_name);
	if( sym && sym->reloc != 0 && check_sym_type(sym))
	{
		LOGX(INFO,"[*] About to perform the hook on relocation 0x%lx",sym->reloc);
		unsigned long res = memory_patch(sym->reloc,hk->rpd_addr);
		if( res > 0 )
		{
			LOGX(INFO,"[+] Replaced symbol's [%s] relocation 0x%lx with 0x%lx",
				sym->name,
				sym->reloc,
				hk->rpd_addr		
			);
			hk->org_addr = res;
			hk->ishooked = true;
		}
	} else
	{
		LOGX(ERROR,"[-] Symbol %s was not found", hk->func_name);
	}

		
}



static bool check_sym_type(Symbol * sym)
{
	return (sym->type == R_GENERIC_JUMP_SLOT) || (sym->type == R_GENERIC_GLOB_DAT);
}
