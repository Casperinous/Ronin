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
#include "elfutils.h"
#include "log.h"
#include "hook.h"

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
}


void __attribute__ ((constructor)) ronin_main()
{

	

	
	LOGX(INFO, "%s","[*]---------------------------------------------");
	LOGX(INFO, "%s","[*] Starting Ronin - Test example targeting libc and connect function.");	
	shared_lib *lib = find_shared_lib("libc.so");

    	if( lib == NULL)
    	{
    		LOGX(ERROR,"[-] Could not find %s", "libc.so" );
    	} else 
	{
		unsigned int (*m_con)(int socket, const struct sockaddr *address, socklen_t address_len) = mconnect;
		unsigned long *val = (unsigned long *)&m_con;
		hook *hk = init_hook_str(*val,"connect","libc.so","");
		if( hk )
		{	
			hook_func(lib,hk);
		}
	}

	LOGX(INFO, "%s","[*]---------------------------------------------");


}
