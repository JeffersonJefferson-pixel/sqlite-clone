#ifndef vm_h 
#define vm_h

#include "compiler.h"
#include "pager.h"

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

// table points to pages of rows and keeps track of how many rows there are
typedef struct {
  Pager* pager;
  uint32_t num_rows;
} Table;

// represents location in the table.
// provides abstraction for how table is stored.
typedef struct {
    Table* table;
    uint32_t row_num;
    bool end_of_table;
} Cursor;

MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table *table);
ExecuteResult execute_statement(Statement* statement, Table* table);

#endif