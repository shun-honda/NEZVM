#include <stdint.h>
#include <stdlib.h>

#ifndef NEZVM_H
#define NEZVM_H

union nezvm_instruction;
typedef union nezvm_instruction nezvm_instruction;

struct nez_ast {
  int refc; // referencing counting gc
  int child_size;
  struct nez_ast **child;
  struct nez_ast *parent;
  long start_pos;
  long end_pos;
  const char *tag;
  const char *value;
};

struct nez_log {
  struct nez_log *next;
  struct nez_ast *child_node;
  int index;
} __attribute__((packed));

struct memory_pool {
  struct nez_ast *object_pool;
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
  struct nez_ast *node;
  struct nez_ast *unused_node;

  int log_stack_size;
  struct nez_log *log_stack;

  struct memory_pool *mpool;

  int symbol_table_size;
  int state_value;
  int state_count;
  struct nez_symbol_table_entry *stacked_symbol_table;

  long *stack_pointer;
  struct nez_ast **node_stack_pointer;
  union nezvm_instruction **call_stack_pointer;
  long *stack_pointer_base;
  struct nez_ast **node_stack_pointer_base;
  union nezvm_instruction **call_stack_pointer_base;
};

union nez_expression;
typedef union nez_expression nez_expression;

typedef struct nez_ast nez_ast;
typedef struct nez_context nez_context;
typedef struct nez_log nez_log;
typedef struct nez_symbol_table_entry nez_symbol_table_entry;
typedef struct memory_pool memory_pool;

nez_ast* create_ast();
void nez_dispose_ast(nez_ast* ast);
nez_context* nez_create_context(char *input_file);
void nez_dispose_context(nez_context* context);
nez_expression* nez_load_grammar(char *syntax_file);
void nez_dispose_expression(nez_expression* expr);
nezvm_instruction* nezvm_compile(nez_expression* expr, char* start_point);
void nezvm_dispose_instruction(nezvm_instruction* inst);

nez_ast* nez_parse(nez_context* context, nezvm_instruction* inst);

#endif /* end of include guard */