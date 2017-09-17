#include <stdlib.h>
#include "thread.h"


thread_vars * 
init_thread_vars()
{
	thread_vars *tv = (thread_vars *)(malloc(sizeof(thread_vars)));
	if( tv )
	{
		set_thread_vars(tv);
	}
	return tv;
}


int
lock_thread_vars_mutex(thread_vars *tv)
{
	/*
	 * Got to finally decide error handling
	 */
	if( !tv ) return -1;
	int res = pthread_mutex_lock(&tv->a_mutex);
	if(res)
	{
		perror("pthread_mutex_lock");
	}
	return res;
}


int
unlock_thread_vars_mutex(thread_vars *tv)
{
	/*
	 * Got to finally decide error handling
	 */
	if( !tv ) return -1;
	int res = pthread_mutex_unlock(&tv->a_mutex);
	if(res)
	{
		perror("pthread_mutex_unlock");
	}
	return res;
}

static void 
set_thread_vars(thread_vars *tv)
{
	pthread_mutex_init(&tv->a_mutex, NULL);
	pthread_cond_init(&tv->a_cond, NULL);
	tv->lock = true;
}

