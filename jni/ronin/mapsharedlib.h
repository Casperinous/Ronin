#ifndef MAPSHAREDLIB_H_   /* Include guard */
#define MAPSHAREDLIB_H_

#define MAPSHAREDLIB_LEN 256
#define BUFFER_VARS_SIZE 6


enum mem_permission { READ = 0x0001, WRITE = 0x0010 , EXECUTE = 0x0100 , MAYSHARE = 0x1000 };


typedef struct map_shared_lib {
	unsigned long base;
	unsigned long end;
	enum mem_permission permissions;
	unsigned long long num;
	char fd[2];
	int fds;
	unsigned long ino;
	char name[MAPSHAREDLIB_LEN];
} map_shared_lib;

map_shared_lib *init_from_str(char *str);



#endif
