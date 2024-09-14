#ifndef pager_h
#define pager_h

#include <stdint.h>
#include "common.h"

// pager accesses page cache and file. 
// table makes requests for pages through the pager.
typedef struct {
  int file_descriptor;
  uint32_t file_length;
  void* pages[TABLE_MAX_PAGES];
}  Pager;

void pager_flush(Pager* pager, uint32_t page_num, uint32_t size);
Pager* pager_open(const char* filename);
void* get_page(Pager* pager, uint32_t page_num);

#endif