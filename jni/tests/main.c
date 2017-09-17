#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>


typedef struct {
	pthread_mutex_t a_mutex;
	pthread_cond_t a_cond;	
	char lock;

} thread_vars;




thread_vars * init_thread_vars()
{
	thread_vars *tv = (thread_vars *)(malloc(sizeof(thread_vars)));
	if( tv )
	{
		/*
		 * tv->a_mutex = PTHREAD_MUTEX_INITIALIZER;
		 * or the below
		 */
		pthread_mutex_init(&tv->a_mutex, NULL);
		pthread_cond_init(&tv->a_cond, NULL);
		tv->lock = 0;
	}
	return tv;
}

static
void thread_vars_lock(thread_vars *tv)
{
	int rc = pthread_mutex_lock(&tv->a_mutex);
	if (rc) { 
		/* an error has occurred */
		perror("pthread_mutex_lock");
	        pthread_exit(NULL);
	}
}


static 
void thread_vars_unlock(thread_vars *tv)
{
	int rc = pthread_mutex_unlock(&tv->a_mutex);
	if (rc) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}
}


void *print_id()
{
	printf("[*] Inside thread, the process id is %d \n", getpid());

}


//Global var for testing
thread_vars *tv = NULL;

void *alter_var()
{
	thread_vars_lock(tv);


	for(int i = 0; i < 50; i++)
	{
		if( i % 5 == 0 )
			printf("%s","\n");
		printf("%d  ",i);
	}

	pthread_cond_signal(&tv->a_cond);
	thread_vars_unlock(tv);

}




int main()
{
	bool bvar = false;
	tv = init_thread_vars(); 
	pthread_t thread, self;
	self = pthread_self();
	//const char *message1 = "Test Thread 1";
	int ret = pthread_create( &thread, NULL, alter_var, NULL);
	thread_vars_lock(tv);
	pthread_cond_wait(&tv->a_cond, &tv->a_mutex);
	printf("LOLALAAALALALA \n");
	thread_vars_unlock(tv);
	free(tv);

	
	return 0;
}
