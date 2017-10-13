#include "storage.h"
#include "memutils.h"
#include "log.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <dirent.h>


char * 
get_external_storage()
{	
	/*
	 * The main logic behind this function is that in order to emulate
	 * the behavior of methods like getExternalDirs(), we loop through 
	 * an array of predefined paths and we check if the directories exist
	 * in our device.
	 */
	
	size_t sz = NELEMS(external_sdcard);
	for(size_t i = 0; i < sz; i++ )
	{
		const char * tmp = external_sdcard[i];
		LOGX(INFO, "[*] Checking for --> %s", tmp);
		if( does_path_exist(tmp) )
			/*
			 * Basically, we can return a pointer to the string,
			 * because it is static pre-allocated. That said, we 
			 * do not risk a UAF attack.
			 */
			return tmp;

	}

	/*
	 * If we do not find any sdkcard related dirs, we need
	 * to look for internal space. Here the search is different:
	 * Starting from an 4.x android version, every user has
	 * his/her own space according to his/her id. We make the * bad *
	 * assumption that, the device will have at most 6 users.
	 */

	sz = NELEMS(internal_sdcard);
	for(size_t i = 0; i < sz; i++)
	{
		const char * tmp = internal_sdcard[i];
		size_t lens = strlen(tmp);
		/*
		 * Well consider that internal space is associated with users and
		 * to be more specific with user ids, so we need to malloc:
		 * - strlen of the path
		 * - one char which will be the user id ( 0 <= userid <= 5 )
		 * - one char which will be the / at the end of the string
		 * - the last will be the NULL/x00 char which will terminate the string.
		 */
		for( size_t x = 0; x < NUSERS; x++)
		{
			char * str = (char *)malloc(sizeof(char) * lens + 1 + 1 + 1);
			strncpy(str,tmp, lens);
			str[lens] = x + '0';
			str[lens + 1] = '/';
			str[lens + 2] = 0x00;
			LOGX(INFO, "[*] Checking path : %s", str);
			if( does_path_exist( str ) )
			{
				return str;
			} else if (ENOENT == errno)
			{
				LOGX(WARN, "[-] Well directory %s does not exist.", str);
			} else if (EACCES == errno)
			{
				LOGX(INFO, "[+] Well, directory %s exists, but we do not have permission to access it!", str);
				return str;
			}

			/*
			 * Well, if it does not exist, just free the string
			 * and begin looping again.
			 */
			free(str);
		}

	}

	return NULL;	
}
bool 
does_path_exist(const char * path)
{
	
	bool res = false;

	DIR* dir = opendir(path); 
	if (dir) 
	{
		/*
		 * Dir exists, so properly close it and return
		 */
		closedir(dir); 
		res = true;
	}

	return res;
}


char * 
get_app_public_path(char * path, char * app_name)
{
	/*
	 * Would be basically a good idea to check for 
	 * path[lens] == "/" but for now we are lazy enough
	 * to skip it ( Also, for NULL too ).
	 */

	/*
	 * Well it is easily assumed that we need to build 
	 * the path related to our app's public folders. We,
	 * have the permissions to create the folder if it does
	 * exist, so we * FULLY * depend on the get_external_storage
	 * for the validity of the path.
	 * Arguments:
	 * - Length of the path ( having as last char the '/' )
	 * - Length of the Android folder ( named "Android" in all versions )
	 * - char '/'
	 * - Length of the data folder ( named "data" in all versions )
	 * - char '/'
	 * - Length of the app's name ( com.example.hellojnicallback ) etc etc
	 * - char '/'
	 * - Last, one char for null termination.
	 */

	size_t lpath  = strlen(path),
	       lapath = strlen(ANDROID_FOLDER),
	       ldata = strlen(ANDROID_DATA),
	       lappname = strlen(app_name);

	size_t lens = lpath + lapath + 1 + ldata + 1 + lappname + 1 + 1;
	char * public_path = (char *)malloc(sizeof(char) * lens);
	if( public_path )
	{
		memset(public_path, 0, lens);
		public_path[lens-1] = 0x00;

		char * idx = public_path;
		/*
	 	* First copy the path
	 	*/
		strncpy(idx, path, lpath);
		//Move pointer to the end.
		idx += lpath;
		LOGX(INFO, "[*] App's public folder with Path folder: %s", public_path);
		
		strncpy(idx, ANDROID_FOLDER, lapath);
		LOGX(INFO, "[*] App's public folder with Android folder: %s", public_path);
		idx += lapath;
		*idx = '/';
		idx += 1;
		LOGX(INFO, "[*] App's public folder with slash : %s", public_path);

		strncpy(idx,ANDROID_DATA, ldata);
		idx += ldata;
		*idx = '/';
		idx += 1;
		
		strncpy(idx, app_name, lappname);
		idx += lappname;
		*idx = '/';
		idx += 1;
		// Finalize it!
		LOGX(INFO, "[*] App's public folder : %s", public_path);
	}

	return public_path;
}

