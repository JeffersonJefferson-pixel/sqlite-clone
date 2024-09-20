#ifndef vm_h 
#define vm_h

#include "compiler.h"

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table *table);
ExecuteResult execute_statement(Statement* statement, Table* table);

#endif