#ifndef CONFIG_H_
#define CONFIG_H_

#include "sll.h"
#include "cJSON/cJSON.h"
#include "hook.h"
#include "log.h"
#include "memutils.h"

typedef struct
{
	const char * config_path;
	const char * name;
	const char *description;
	slinked_list_t * hooks;

} config;


config * 
parse_from_file(const char * config);

static char * 
parse_string_from_obj(cJSON * obj, const char * identifier);



static slinked_list_t * 
parse_hooks_from_json(cJSON * arr);


static slinked_node_t * 
parse_hook_from_obj(cJSON * jhook);


#endif
