#ifndef MEMUTILS_H_
#define MEMUTILS_H_

#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

//stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define ALIGN_PTR(ps,ptr) (ptr & ~(ps - 1))

/*
 * Functions taken straigh from android source code.
 */


static off64_t kPageMask = ~((off64_t)(PAGE_SIZE-1));

off64_t 
page_start(off64_t offset);

size_t 
page_offset(off64_t offset);




void 
free_arr(void **arr, int size);

bool 
copy_str(char * source, char * dest, size_t length);


unsigned long 
memory_patch(unsigned long original, unsigned long value);

unsigned long *
get_func_addr(const char * so, const char * func, bool isloaded);



#endif
