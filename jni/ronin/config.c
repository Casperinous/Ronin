#include "config.h"
#include "sll.h"
#include "cJSON/cJSON.h"
#include "hook.h"
#include "log.h"
#include "memutils.h"
#include <stdlib.h>
#include <string.h>

config * 
parse_from_file(const char * js)
{
	config * conf = NULL;

	if( !js )
	{
		return conf;
	}
	
	cJSON * root = cJSON_Parse(js);
	if( root )
	{	
		conf = (config *)malloc(sizeof(config));
		conf->name = parse_string_from_obj(root,"name");
		conf->description = parse_string_from_obj(root,"description");
		
		cJSON *jhooks =	cJSON_GetObjectItemCaseSensitive(root,"hooks"); 
		if( jhooks )
		{
			conf->hooks = parse_hooks_from_json(jhooks);
		} else 
		{
			conf->hooks = NULL;
		}

	}

	return conf;

}

static char * parse_string_from_obj(cJSON * obj, const char * identifier)
{
	char * str = NULL;

	if( !obj || !identifier )
	{
		return str;
	}
	
	cJSON * jstr = cJSON_GetObjectItemCaseSensitive(obj, identifier);
	if ( jstr )
	{
		if(copy_str(jstr->valuestring, &str, strlen(jstr->valuestring)))
			return str;
	}

	return str;


}


static slinked_list_t * 
parse_hooks_from_json(cJSON * arr)
{
	slinked_list_t *list = NULL;
	if( !arr )
	{
		return list;
	}

	list = init_slist_t();
	
	for (int i = 0; i < cJSON_GetArraySize(arr); i++)     
	{
		cJSON *jhook = cJSON_GetArrayItem(arr, i);
		if(jhook)
		{
			slinked_node_t *node = parse_hook_from_obj(jhook);
			if( node )
			{
				if( slinked_list_append(list,node) )
				{
					LOGX(INFO,"[+] Success in appending node at address %p", node);
				}
			}
		}
	}
	
	return list;
}


static slinked_node_t * 
parse_hook_from_obj(cJSON * jhook)
{
	slinked_node_t * node = NULL;
	
	
	cJSON *org_func = cJSON_GetObjectItemCaseSensitive(jhook, "org_func");
	cJSON *org_so = cJSON_GetObjectItemCaseSensitive(jhook, "org_so");
	cJSON *trg_func = cJSON_GetObjectItemCaseSensitive(jhook, "trg_func");
	cJSON *trg_so = cJSON_GetObjectItemCaseSensitive(jhook, "trg_so");

	/*
	 * Probably we need more checks here. Will be added later
	 */
	if( org_func && org_so && trg_func && trg_so )
	{
		node = (slinked_node_t *)(malloc(sizeof(slinked_node_t)));
		node->data = (hook *)init_hook_str(
				org_func->valuestring,
				trg_func->valuestring,
				org_so->valuestring,
				trg_so->valuestring );
		/*
		 * Well, because we init here, we set it
		 * here * temporary *. Of course in *
		 * append operations this changes
		 */
		node->next = NULL;
	}

	return node;
}

