#ifndef APP_H
#define APP_H


#include <stdbool.h>

typedef struct
{
	char * appname;
	char * apppath;
	bool is64;
} app;
 

app * 
get_app_info();

static char * 
get_app_cmdline();


#endif
