#ifndef pager_h
#define pager_h

#include <stdint.h>
#include "common.h"

// flush a page to disk.
void pager_flush(Pager* pager, uint32_t page_num);
Pager* pager_open(const char* filename);
void* get_page(Pager* pager, uint32_t page_num);

#endif