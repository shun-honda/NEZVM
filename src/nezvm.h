#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef NEZVM_H
#define NEZVM_H

union nezvm_instruction;
typedef union nezvm_instruction nezvm_instruction;

struct nez_node {
  int refc; // referencing counting gc
  int child_size;
  struct nez_node **child;
  struct nez_node *parent;
  long start_pos;
  long end_pos;
  const char *tag;
  const char *value;
};

struct nez_log {
  struct nez_log *next;
  struct nez_node *child_node;
  int index;
} __attribute__((packed));

struct memory_pool {
  struct nez_node *object_pool;
  struct nez_log *log_pool;
  size_t oidx;
  size_t lidx;
  size_t init_size;
};

struct nez_symbol_table_entry {
  int table_type; // T in <def T e>
  int utf8_length;
  char *utf8;
};

struct nez_context {
  char *inputs;
  size_t input_size;
  long pos;
  struct nez_node *node;
  struct nez_node *unused_node;

  int log_stack_size;
  struct nez_log *log_stack;

  struct memory_pool *mpool;

  int symbol_table_size;
  int state_value;
  int state_count;
  struct nez_symbol_table_entry *stacked_symbol_table;

  long *stack_pointer;
  struct nez_node **node_stack_pointer;
  union nezvm_instruction **call_stack_pointer;
  long *stack_pointer_base;
  struct nez_node **node_stack_pointer_base;
  union nezvm_instruction **call_stack_pointer_base;
};

#define NEZVM_MAX_STACK_LENGTH 1024

union nez_expression;
typedef union nez_expression nez_expression;

typedef struct nez_node nez_node;
typedef struct nez_context nez_context;
typedef struct nez_log nez_log;
typedef struct nez_symbol_table_entry nez_symbol_table_entry;
typedef struct memory_pool memory_pool;

nez_node* create_node();
void nez_dispose_node(nez_node* node);
nez_context* nez_create_context(const char *input_file);
void nez_dispose_context(nez_context* context);
nez_expression* nez_load_grammar(const char *syntax_file);
void nez_dispose_expression(nez_expression* expr);
nezvm_instruction* nezvm_compile(nez_expression* expr, char* start_point);
void nezvm_dispose_instruction(nezvm_instruction* inst);

char *load_file(const char *filename, size_t *length);

void nez_print_error_info(const char *errmsg);
nez_node* nez_parse(nez_context* context, nezvm_instruction* inst);

#endif /* end of include guard */