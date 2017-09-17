#include "jvm.h"
#include <stdlib.h>




jvm * 
init_jvm_str(JavaVM *vm, JNIEnv * env)
{
	jvm * tmp = (jvm *)malloc(sizeof(jvm));
	if( tmp )
	{
		tmp->vm = vm;
		tmp->env = env;
	}
	return tmp;
}
