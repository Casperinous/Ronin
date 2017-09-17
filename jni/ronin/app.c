#include "app.h"
#include "log.h"
#include "memutils.h"
#include "stdlib.h"
#include <sys/types.h>
#include <unistd.h>




app * 
get_app_info()
{
	app * ap = (app *)malloc(sizeof(app));
	if( ap )
	{
		get_app_cmdline(ap);
	}	

}


static void 
get_app_cmdline(app * ap)
{
	char buffer[2048], cmd[2048];
	FILE *fp = NULL;
	sprintf( cmd, "/proc/%d/cmdline", getpid() );
	fp = fopen( cmd, "rt" );
	while( fgets( buffer, sizeof(buffer), fp ) )
	{
		LOGX(INFO,"[+] Proccess is started with these commands : %s", buffer);
		if(copy_str(buffer, ap->apppath, NELEMS(buffer)))
		{
			LOGX(INFO,"[+] Succesfuly setting App's path name %s", ap->apppath);
			break;
		}
		
	}	
}
