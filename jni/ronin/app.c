#include "app.h"
#include "log.h"
#include "memutils.h"
#include "stdlib.h"
#include <sys/types.h>
#include <unistd.h>
#include <string.h>




app * 
get_app_info()
{
	app * ap = (app *)malloc(sizeof(app));
	ap->apppath = NULL;
	if( ap )
	{
		ap->appname = get_app_cmdline();
	}
	return ap;	

}


static char *
get_app_cmdline()
{
	char buffer[2048], cmd[2048];
	FILE *fp = NULL;
	char * tmp = NULL;
	sprintf( cmd, "/proc/%d/cmdline", getpid() );
	fp = fopen( cmd, "rt" );
	while( fgets( buffer, sizeof(buffer), fp ) )
	{
		LOGX(INFO,"[+] Proccess is started with these commands : %s length :> %d", buffer, strlen(buffer));
		copy_str(buffer,&tmp, strlen(buffer) + 1);
		
	}
	fclose(fp);	
	return tmp;
}
