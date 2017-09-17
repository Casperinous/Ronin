#include "hash.h"
#include "symbol.h"
#include <stdlib.h>


dt_hash * 
init_dt_hash(unsigned long load_bias, uint64_t dptr)
{
	
	
	/*
	 * No checks, as from that point we know for sure that
	 * we have a load_bias != 0 and we have located Dyn sections.
	 */
	dt_hash * hash = (dt_hash *)(malloc(sizeof(dt_hash)));
	
	//TODO Error handling - reminder again.
	
	if( hash == NULL ) return hash;
	
	/*
	 * Double Parentheses, like for real, it gonna force me
	 * to switch to reinterpeter cast. It is THAT ugly to read.
	 */
	hash->nbucket = ((uint32_t *)(load_bias + dptr))[0];
	hash->nchain = ((uint32_t *)(load_bias + dptr))[1];
	hash->bucket = (uint32_t *)(load_bias + dptr + 8);
	hash->chain = (uint32_t *)(load_bias + dptr + 8 + hash->nbucket * 4);

	return hash;
}

dt_gnu_hash *
init_dt_gnu_hash(unsigned long load_bias, uint64_t dptr)
{
	dt_gnu_hash * hash = (dt_gnu_hash *)(malloc(sizeof(dt_gnu_hash)));
	
	if( hash == NULL ) return hash;

	hash->nbucket = ((uint32_t *)(load_bias + dptr))[0];
	hash->maskwords = ((uint32_t *)(load_bias + dptr))[2];
	hash->shift2 = ((uint32_t *)(load_bias + dptr))[3];
	hash->bloom_filter = (uint32_t *)(load_bias + dptr + 16);
	hash->bucket = (uint32_t *)(hash->bloom_filter + hash->maskwords);
	hash->chain = hash->bucket + hash->nbucket - ((uint32_t *)(load_bias + dptr))[1];

	--hash->maskwords;
	
	return hash;
}


uint32_t 
calc_gnu_hash(const char * name_)
{
	uint32_t res = 0;
	if (name_) 
	{
	    uint32_t h = 5381;
	    const uint8_t* name = (const uint8_t *)(name_);
	    while (*name != 0) 
	    {
		h += (h << 5) + *name++; // h*33 + c = h + h * 32 + c = h + h << 5 + c
	    }
	    
	    res =  h;
	} 

	return res;

}

uint32_t 
calc_elf_hash(const char * name_) 
{
	uint32_t res = 0;

	if( name_ )
	{
		const uint8_t* name_bytes = (const uint8_t*)(name_);
		uint32_t h = 0, g;
		while (*name_bytes) 
  		{
			h = (h << 4) + *name_bytes++;
			g = h & 0xf0000000;
			h ^= g;
			h ^= g >> 24;
  		}

		res = h;
	}
	return res;
}


