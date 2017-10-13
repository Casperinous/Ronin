#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <elf.h>
#include <stdbool.h>
#include "sharedlib.h"     
#include <sys/socket.h>
#include <jni.h>
#include <android/asset_manager.h>
#include "elfutils.h"
#include "log.h"
#include "hook.h"
#include "storage.h"
#include "config.h"
#include "app.h"
#include "sll.h"

unsigned int mconnect(int socket, const struct sockaddr *address, socklen_t address_len)
{
	
	unsigned int res = 0;
	LOGX(INFO, "%s","[*] Inside the modified connect function of libc");
	
	void *libc = (void *)dlopen("libc.so",0);
	if( libc )
	{
		unsigned int (*org_connect)(int socket, const struct sockaddr *address, socklen_t address_len);
		/*
	 	* Based on linux doc, this is the current best way.
	 	*/
		*(unsigned int **)(&org_connect) = (unsigned int *) dlsym(libc, "connect");
		res = org_connect(socket,address,address_len);
	}

	return res;

}



jint JNICALL JNI_OnLoad(JavaVM* vm, void* aReserved)
{
	
	JNIEnv* env;
	jint res = -1;
	if( vm )
	{
		LOGX(INFO,"[+] JavaVM address --> %p", vm);
	} else
	{
		LOGX(ERROR, "%s","[-] JavaVM pointer is null :/");
		return -1;
	}


	if ((*vm)->GetEnv(vm,(void**)(&env), JNI_VERSION_1_6) != JNI_OK) {
		LOGX(ERROR, "%s", "[-] Could not set JavaEnv variable.");
		return -1;
	}
	
	return JNI_VERSION_1_6;

	LOGX(INFO, "Size of JavaVM struct is %zu", sizeof(JavaVM));
}


void __attribute__ ((constructor)) ronin_main()
{

	

	
	LOGX(INFO, "%s","[*]---------------------------------------------");
	LOGX(INFO, "%s","[*] Starting Ronin - Test example targeting libc and connect function.");	
	
	
	char * path = get_external_storage();
	if( path )
	{
		app * ap = get_app_info();
		if( ap->appname )
		{
			LOGX(INFO,"[*] App's name :> %s", ap->appname);
			char * app_public_folder = get_app_public_path(path, ap->appname);
			if( app_public_folder  )
			{
				char buffer[4096], path[2048];         
				FILE *fp = NULL;         
				sprintf( path , "%s%s",app_public_folder, "config.json" );
				LOGX(INFO,"[*] Path of the config file :> %s", path );
				fp = fopen(path, "r+");
				if( fp )
				{
					int result = fread (buffer, 1, 4096, fp);
					LOGX(INFO,"[*] We read %d bytes and content is %s", result, buffer);
					config* conf = parse_from_file(buffer);
					slinked_node_t * tmp = conf->hooks->head;
					while( tmp != NULL) 
					{      	
					   	hook * htmp = (hook *)tmp->data;
						LOGX(INFO,"[+] Got Hook at address [%p] with data :> %s %s %s %s", htmp, htmp->func_name, htmp->exp_name, htmp->trg_so, htmp->exp_so);	
						tmp = tmp->next;
						break; 
					}
					LOGX(INFO,"%s","[*] Finished printing hooks. Exiting .....");

				}
				fclose(fp);
			}
		}
	}


	
	
	
	
	shared_lib *lib = find_shared_lib("libc.so");

    	if( lib == NULL)
    	{
    		LOGX(ERROR,"[-] Could not find %s", "libc.so" );
    	} else 
	{
		/*
		unsigned int (*m_con)(int socket, const struct sockaddr *address, socklen_t address_len) = mconnect;
		unsigned long *val = (unsigned long *)&m_con;
		hook *hk = init_hook_str(*val,"connect","m_con","libc.so","");
		if( hk )
		{	
			hook_func(lib,hk);
		}*/
	}

	LOGX(INFO, "%s","[*]---------------------------------------------");
	
}
