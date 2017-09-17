#include "mapsharedlib.h"
#include "stringutils.h"
#include "memutils.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>


static enum mem_permission
get_mem_permission(char c)
{
	enum mem_permission mp = 0;
	switch(c)
	{
		case 'r':
			mp = READ;
		break;
		case 'w':
			mp = WRITE;
		break;
		case 'x':
			mp = EXECUTE;
		break;
		case 's':
			mp = MAYSHARE;
		break;
	}

	return mp;
	
}

map_shared_lib *
init_from_str(char *str)
{

	map_shared_lib *lib = (map_shared_lib *)malloc(sizeof(map_shared_lib));

	if( str == NULL )
		return lib;

	char **strs = split(str," ", BUFFER_VARS_SIZE);
	
	char **addrs = NULL;
	if( strs && strs[0] )
	{
		addrs = split(strs[0], "-", 2);
		if( addrs )
		{
			
			lib->base = strtoul(addrs[0], NULL, 16);
			lib->end  = strtoul(addrs[1], NULL, 16);
			free_arr(addrs,2);
		}
	} else 
	{
		LOGX(ERROR,"[-] %s","Something went really wrong ... \n");
		return NULL;
	}

	int idx = 0;
	for(idx = 0; idx < strlen(strs[1]); idx++)
	{
		lib->permissions |= get_mem_permission(strs[1][idx]);
	}

	lib->num = strtoul(strs[2], NULL, 16);
	char **fd = split(strs[3],":",2);
	if( fd )
	{
		strlcpy(lib->fd,fd[0],strlen(fd[0]));
		lib->fds = strtol(fd[1], NULL, 16);

		free_arr(fd,2);
	}

	lib->ino = strtoul(strs[4], NULL, 10);
	strlcpy(lib->name, strs[5], strlen(strs[5]));
	
	free_arr(strs, BUFFER_VARS_SIZE);

	LOGX(INFO,"[*] Segment Info > Start : 0x%lx End : 0x%lx Perm : %d Fd : %s Fdn : %d Ino : %lu Name: %s \n", 
			lib->base, 
			lib->end, 
			lib->permissions,
			lib->fd,
			lib->fds,
			lib->ino,
			lib->name
			);

	return lib;

}




