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

// get next leaf field
uint32_t* leaf_node_next_leaf(void* node) {
    return node + LEAF_NODE_NEXT_LEAF_OFFSET;
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

void set_node_root(void* node, bool is_root) {
    uint8_t value = is_root;
    *(uint8_t*)(node + IS_ROOT_OFFSET) = value;
}

void initialize_leaf_node(void* node) {
    set_node_type(node, NODE_LEAF);
    set_node_root(node, false);
    *leaf_node_num_cells(node) = 0;
    *leaf_node_next_leaf(node) = 0;
}

NodeType get_node_type(void* node) {
    uint8_t node_type = *(uint8_t*)(node + NODE_TYPE_OFFSET);
    return (NodeType)node_type;
} 

uint32_t* internal_node_num_keys(void* node) {
    return node + INTERNAL_NODE_NUM_KEYS_OFFSET;
}

uint32_t* internal_node_right_child(void* node) {
    return node + INTERNAL_NODE_RIGHT_CHILD_OFFSET;
}

uint32_t* internal_node_cell(void* node, uint32_t cell_num) {
    return node + INTERNAL_NODE_HEADER_SIZE + cell_num * INTERNAL_NODE_CELL_SIZE;
}

uint32_t* internal_node_child(void* node, uint32_t child_num) {
    uint32_t num_keys = *internal_node_num_keys(node);
    if (child_num > num_keys) {
        printf("Tried to access child_num %d > num_keys %d\n", child_num, num_keys);
        exit(EXIT_FAILURE);
    } else if (child_num == num_keys) {
        uint32_t* right_child  = internal_node_right_child(node);
        // prevent accessing an invalid page.
        if (*right_child == INVALID_PAGE_NUM) {
            printf("Tried to access right child of node, but was invalid page\n");
            exit(EXIT_FAILURE);
        }
        return right_child;
    } else {
        uint32_t* child = internal_node_cell(node, child_num);
        // prevent access accessing invalid page
        if (*child == INVALID_PAGE_NUM) {
            printf("Tried to access child %d of node, but was invalid page\n", child_num);
            exit(EXIT_FAILURE);
        }
        return child;
    }
}

uint32_t* internal_node_key(void* node, uint32_t key_num) {
    return (void*)internal_node_cell(node, key_num) + INTERNAL_NODE_CHILD_SIZE;
}

uint32_t internal_node_find_child(void* node, uint32_t key) {
  uint32_t num_keys = *internal_node_num_keys(node);

  // find index of child to search with binary search.
  uint32_t min_index = 0;
  uint32_t max_index = num_keys;

  while (min_index < max_index) {
    uint32_t index = (min_index + max_index) / 2;
    uint32_t key_to_right = *internal_node_key(node, index);
    if (key_to_right > key) {
      max_index = index;
    } else {
      min_index = index + 1;
    }
  }

  return min_index;
}

void update_internal_node_key(void *node, uint32_t old_key, uint32_t new_key) {
    // find internal node with old key
    uint32_t old_child_index = internal_node_find_child(node, old_key);
    // update to new key
    *internal_node_key(node, old_child_index) = new_key;
}

uint32_t get_node_max_key(Pager* pager, void* node) {
  // walk down the right children until get to a leaf node.
  if (get_node_type(node) == NODE_LEAF) {
    return *leaf_node_key(node, *leaf_node_num_cells(node) - 1);
  }
  void* right_child = get_page(pager, *internal_node_right_child(node));
  return get_node_max_key(pager, right_child);
}

bool is_node_root(void* node) {
    uint8_t value = *(uint8_t*)(node + IS_ROOT_OFFSET);
    return (bool)value;
}

void initialize_internal_node(void* node) {
    set_node_type(node, NODE_INTERNAL);
    set_node_root(node, false);
    *internal_node_num_keys(node) = 0;
    // if not set, right child might be 0, the root page number. 
    *internal_node_right_child(node) = INVALID_PAGE_NUM;
}

uint32_t* node_parent(void * node) {
    return node + PARENT_POINTER_OFFSET;
}