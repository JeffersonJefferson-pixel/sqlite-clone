#include <stdlib.h>
#include <string.h>

#include "btree.h"
#include "pager.h"

// accessing leaf node fields
uint32_t* leaf_node_num_cells(void* node) {
    return node + LEAF_NODE_NUM_CELLS_OFFSET;
}

void* leaf_node_cell(void* node, uint32_t cell_num) {
    return node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
}

uint32_t* leaf_node_key(void* node, uint32_t cell_num) {
    return leaf_node_cell(node, cell_num);
}

void* leaf_node_value(void* node, uint32_t cell_num) {
    return leaf_node_cell(node, cell_num) + LEAF_NODE_KEY_SIZE;
}

static void set_node_type(void* node, NodeType type) {
    uint8_t value = type;
    *(uint8_t*)(node + NODE_TYPE_OFFSET) = value;
}

void initialize_leaf_node(void* node) {
    *leaf_node_num_cells(node) = 0;
    set_node_type(node, NODE_LEAF);
}

NodeType get_node_type(void* node) {
    uint8_t node_type = *(uint8_t*)(node + NODE_TYPE_OFFSET);
    return (NodeType)node_type;
}