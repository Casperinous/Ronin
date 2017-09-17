#ifndef THREAD_H_
#define THREAD_H_


#include <stdlib.h>             
#include <stdio.h>              
#include <sys/types.h>          
#include <unistd.h>             
#include <pthread.h>            
#include <stdbool.h>            
                                
                                
typedef struct {                
	pthread_mutex_t a_mutex;
	pthread_cond_t a_cond;  
	bool lock;              
				                                
} thread_vars;      



thread_vars * 
init_thread_vars();

int
lock_thread_vars_mutex(thread_vars *tv);


int
unlock_thread_vars_mutex(thread_vars *tv);

static void 
set_thread_vars(thread_vars *tv);

#endif
 
