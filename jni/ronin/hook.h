#ifndef HOOK_H_
#define HOOK_H_

#include <stdlib.h>
#include <stdbool.h>
#include "sharedlib.h"
#include "symbol.h"

typedef struct {

	unsigned long org_addr;
	unsigned long rpd_addr;
	char * func_name;
	char * exp_name;
	char * trg_so;
	char * exp_so;
	bool ishooked;

} hook;


hook * 
init_hook_str(char * func_name, char * exp_name, char * trg_so, char * exp_so);

void 
hook_func(shared_lib * lib, hook * hk);

static bool check_sym_type(Symbol * sym);

#endif 
