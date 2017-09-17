#include "stringutils.h"
#include "log.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char ** 
split(char * str, char * delimeter, int num_of_elements)
{
	char **array = NULL;
	char *pch = NULL;
	uint8_t idx = 0;

	if(str == NULL || delimeter == NULL ) return array;
	/*
	 * Seven as a number to be defined as a constant
	 */
	array = (char **)( malloc(sizeof(char*) * num_of_elements) );
  	if(array == NULL )
	{
	       	LOGX(ERROR,"%s","[-] Could not allocated memory.");
		return array;
	}
  	pch = strtok (str, delimeter);
  	while (pch != NULL)
  	{
  		size_t length = strlen(pch);
		array[idx] = (char *)( malloc(sizeof(char) * (length + 1) ) );
  		strlcpy(array[idx], pch, length + 1);
  		array[idx][length] = '\0';
		idx++;
    		pch = strtok (NULL, delimeter);
  	}

  	return array;

}
