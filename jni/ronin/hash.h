#ifndef HASH_H_
#define HASH_H_

#include <stdlib.h>

typedef struct {

	size_t nbucket;
	size_t nchain;
	uint32_t *bucket;
	uint32_t *chain;
} dt_hash;

typedef struct {

	size_t nbucket;
	uint32_t  maskwords;
	uint32_t  shift2;
	uint32_t *bloom_filter;
	uint32_t *bucket;
	uint32_t *chain;
} dt_gnu_hash;

dt_hash * 
init_dt_hash(unsigned long load_bias, uint64_t dptr);

dt_gnu_hash *
init_dt_gnu_hash(unsigned long load_bias, uint64_t dptr);

uint32_t 
calc_gnu_hash(const char * name_);

uint32_t 
calc_elf_hash(const char * name_);


#endif
