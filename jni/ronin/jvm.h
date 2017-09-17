#ifndef JVM_H_
#define JVM_H_

#include <jni.h>
#include <stdlib.h>

typedef struct 
{
	JavaVM *vm;
	JNIEnv * env;

} jvm;



jvm * 
init_jvm_str(JavaVM *vm, JNIEnv * env);


#endif
