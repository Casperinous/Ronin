#ifndef LOG_H_
#define LOG_H_

#include <android/log.h>
#define APPNAME "Ronin"
#define kTAG "Ronin"
#define LOGX(log_type,...) \
	  do { if(DEBUG) ((void)__android_log_print(log_type, kTAG, __VA_ARGS__)); } while(0)



typedef enum {
	DEBUG = ANDROID_LOG_DEBUG,
	ERROR = ANDROID_LOG_ERROR,
	INFO = ANDROID_LOG_INFO,
	VERBOSE = ANDROID_LOG_VERBOSE,
	WARN = ANDROID_LOG_WARN
} log_type;



#endif
