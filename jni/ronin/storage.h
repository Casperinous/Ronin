#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdbool.h>

/*
 * Well, we * shamefully * bet that the device
 * will have at most 6 different users.
 */
#define NUSERS 5
#define ANDROID_FOLDER "Android"
#define ANDROID_DATA "data"


typedef struct storage {

	char * mdevice;

} storage;




static const char * external_sdcard[] = { 
	
	"/sdcard/",
	"/mnt/sdcard/",
	/*
	 * We can all blame Samsung for this.
	 * https://stackoverflow.com/questions/23123461/external-sdcard-file-path-for-android
	 */
	"/mnt/extSdCard/"	
};

static const char * internal_sdcard[] = {
	
	"/storage/emulated/",
	"/storage/sdcard/"
	//https://android.googlesource.com/platform/frameworks/base/+/android-5.0.0_r6/core/java/android/os/Environment.java#125
};



char * 
get_external_storage();


bool 
does_path_exist(const char * path);

char * 
get_app_public_path(char * path, char * app_name);

#endif
