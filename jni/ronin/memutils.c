#include "memutils.h"
#include <stdlib.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "log.h"


size_t 
page_offset(off64_t offset) {
	return (size_t)(offset & (PAGE_SIZE-1));
}



off64_t 
page_start(off64_t offset) {
	return offset & kPageMask;
}



void
free_arr(void **arr, int size)
{
	for(int i = 0; i < size; i++)
		if( arr[i] )
			free(arr[i]);
}

bool
copy_str(char * source, char ** dest, size_t length)
{
	
	if ( !source || length <= 0 ) return false;

	bool res = false;

	*dest = (char *)malloc(sizeof(char) * (length + 1));
	if( *dest )
	{
		strlcpy(*dest, source, length);
		*(dest + length) = 0x00;
		LOGX(INFO,"Pointer :> %p content :> %s",*dest, *dest);
		res = true;
	}

	return res;


}


unsigned long
memory_patch(unsigned long original, unsigned long value)
{
	unsigned long res = -1;
	size_t pagesize = sysconf(_SC_PAGESIZE);
	unsigned long *aligned_pointer = (unsigned long*)ALIGN_PTR(pagesize,original);
	mprotect(aligned_pointer, pagesize, PROT_WRITE | PROT_READ);
	res = *(unsigned long*)original;
	unsigned long *adr = (unsigned long *)original;
	*adr = value; 
	mprotect(aligned_pointer, pagesize, PROT_READ);
	
#if defined(__arm__) || defined(__aarch64__)
	/*
	 * Flush Cache in ARM devices - to be honest, i did not notice any difference
	 */
	//https://issuetracker.google.com/issues/36906645	
	unsigned long *end = (unsigned long *)adr + pagesize;
	syscall(0xf0002, adr, end);
#endif
	return res;

}


unsigned long *
get_func_addr(const char * so, const char * func, bool isloaded)
{
	
	unsigned long * addr = NULL;

	if( !so || !func ) return addr;

	int mode = isloaded ? RTLD_NOLOAD : RTLD_NOW;

	void *lib = (void *)dlopen(so, mode);
	if( lib )
	{
		addr = (unsigned long *)dlsym(lib, func);
	}

	return addr;
}
