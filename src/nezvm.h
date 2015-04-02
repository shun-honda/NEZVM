#include <stdint.h>
#include <stdlib.h>
#include "nez_bitset.c"
#include "arraylist.h"
#include "hashmap.h"

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

typedef struct nez_expr_base {
  int expr;
  size_t size;
  int id;
} nez_expr_base;

typedef struct nez_tag_entry {
  int id;
  char *tag_name;
} nez_tag_entry;

typedef struct nez_tag {
  array_list* list;
  map_t map;
} nez_tag;

nez_tag* nez_new_tag();
void nez_dispose_tag(nez_tag* tag);
nez_tag_entry* tag(nez_tag* tag, char* name);
nez_tag_entry* tag_id(nez_tag* tag, int id);
nez_tag_entry* nez_new_tag_entry(nez_tag* tag, char* name);
void nez_dispose_tag_entry(void* tag_entry);

typedef bitset_t *bitset_ptr_t;

#define NEZ_MAX_EXPR 23
#define NEZ_EXPR_EACH(EXPR)\
  EXPR(sequence)\
  EXPR(choice)\
  EXPR(new)\
  EXPR(left_join)\
  EXPR(option)\
  EXPR(repetition)\
  EXPR(not)\
  EXPR(and)\
  EXPR(link)\
  EXPR(tagging)\
  EXPR(value)\
  EXPR(char)\
  EXPR(charset)\
  EXPR(string)\
  EXPR(any)\
  EXPR(non_terminal)\
  EXPR(def)\
  EXPR(defindent)\
  EXPR(is)\
  EXPR(isa)\
  EXPR(indent)\
  EXPR(if)\
  EXPR(with)\
  EXPR(without)\

#define NEZ_SEQUENCE 0
struct nez_sequence{
  nez_expr_base base;
  union nez_expression** list;
};
typedef struct nez_sequence nez_sequence;

#define NEZ_CHOICE 1
struct nez_choice{
  nez_expr_base base;
  union nez_expression** list;
};
typedef struct nez_choice nez_choice;

#define NEZ_NEW 2
struct nez_new{
  nez_expr_base base;
  union nez_expression** list;
};
typedef struct nez_new nez_new;

#define NEZ_LEFT_JOIN 3
struct nez_left_join{
  nez_expr_base base;
  union nez_expression** list;
};
typedef struct nez_left_join nez_left_join;

#define NEZ_OPTION 4
struct nez_option{
  nez_expr_base base;
  union nez_expression *inner;
};
typedef struct nez_option nez_option;

#define NEZ_REPETITION 5
struct nez_repetition{
  nez_expr_base base;
  union nez_expression *inner;
};
typedef struct nez_repetition nez_repetition;

#define NEZ_NOT 6
struct nez_not{
  nez_expr_base base;
  union nez_expression *inner;
};
typedef struct nez_not nez_not;

#define NEZ_AND 7
struct nez_and{
  nez_expr_base base;
  union nez_expression *inner;
};
typedef struct nez_and nez_and;

#define NEZ_LINK 8
struct nez_link{
  nez_expr_base base;
  union nez_expression *inner;
};
typedef struct nez_link nez_link;

#define NEZ_TAGGING 9
struct nez_tagging{
  nez_expr_base base;
  nez_tag_entry* tag;
};
typedef struct nez_tagging nez_tagging;

#define NEZ_VALUE 10
struct nez_value{
  nez_expr_base base;
  char* value;
};
typedef struct nez_value nez_value;

#define NEZ_CHAR 11
struct nez_char{
  nez_expr_base base;
  char c;
};
typedef struct nez_char nez_char;

#define NEZ_CHARSET 12
struct nez_charset{
  nez_expr_base base;
  bitset_ptr_t set;
};
typedef struct nez_charset nez_charset;

#define NEZ_STRING 13
struct nez_string{
  nez_expr_base base;
  char* str;
};
typedef struct nez_string nez_string;

#define NEZ_ANY 14
struct nez_any{
  nez_expr_base base;
};
typedef struct nez_any nez_any;

#define NEZ_NON_TERMINAL 15
struct nez_non_terminal{
  nez_expr_base base;
  char* rule_name;
};
typedef struct nez_non_terminal nez_non_terminal;

#define NEZ_DEF 16
struct nez_def{
  nez_expr_base base;
  union nez_expression *inner;
  nez_tag_entry* table;
};
typedef struct nez_def nez_def;

#define NEZ_DEFINDENT 17
struct nez_defindent{
  nez_expr_base base;
};
typedef struct nez_defindent nez_defindent;

#define NEZ_IS 18
struct nez_is{
  nez_expr_base base;
  nez_tag_entry* table;
  union nez_expression* symbol_expression;
};
typedef struct nez_is nez_is;

#define NEZ_ISA 19
struct nez_isa{
  nez_expr_base base;
  nez_tag_entry* table;
  union nez_expression* symbol_expression;
};
typedef struct nez_isa nez_isa;

#define NEZ_INDENT 20
struct nez_indent{
  nez_expr_base base;
};
typedef struct nez_indent nez_indent;

#define NEZ_IF 21
struct nez_if{
  nez_expr_base base;
  char *flag_name;
};
typedef struct nez_if nez_if;

#define NEZ_WITH 22
struct nez_with{
  nez_expr_base base;
  union nez_expression *inner;
  char* flag_name;
};
typedef struct nez_with nez_with;

#define NEZ_WITHOUT 23
struct nez_without{
  nez_expr_base base;
  union nez_expression *inner;
  char* flag_name;
};
typedef struct nez_without nez_without;

typedef union nez_expression {
#define DEF_NEZ_EXPR_UNION(EXPR) nez_##EXPR _##EXPR;
  NEZ_EXPR_EACH(DEF_NEZ_EXPR_UNION);
#undef DEF_NEZ_EXPR_UNION
  nez_expr_base base;
} nez_expression;

struct nez_rule {
  char* rule_name;
  nez_expression* expr;
};

struct nez_grammar {
  char* input_file_name;
  size_t rule_list_size;
  struct nez_rule *rule_list;
};

typedef struct nez_node nez_node;
typedef struct nez_context nez_context;
typedef struct nez_log nez_log;
typedef struct nez_symbol_table_entry nez_symbol_table_entry;
typedef struct memory_pool memory_pool;
typedef struct nez_grammar nez_grammar;
typedef struct nez_rule nez_rule;

nez_node* create_node();
void nez_dispose_node(nez_node* node);
nez_context* nez_create_context(const char *input_file);
void nez_dispose_context(nez_context* context);
nez_grammar* nez_load_grammar(const char *syntax_file);
nez_grammar* nez_create_grammar(const char *syntax_file, nez_rule *rule_list, size_t rule_list_size);
nez_node* nez_parse_grammar(char *syntax);
void nez_dispose_grammar(nez_grammar* nez);
nez_rule* nez_create_rules(size_t* rule_list_size);
void nez_dispose_rules(nez_rule *rule_list, size_t rule_list_size);
nezvm_instruction* nezvm_compile(nez_grammar* nez, char* start_point);
void nezvm_dispose_instruction(nezvm_instruction* inst);

#define nez_new_sequence(...)\
  (nez_expression[][]){ __VA_ARGS__ },\
  sizeof((nez_expression[][]){ __VA_ARGS__ }) / sizeof(nez_expression)\
  )
nez_expression* _nez_new_sequence(nez_expression **expr, size_t size);
void nez_dispose_sequence(nez_expression *expr);
#define nez_new_choice(...)\
  (nez_expression[][]){ __VA_ARGS__ },\
  sizeof((nez_expression[][]){ __VA_ARGS__ }) / sizeof(nez_expression)\
  )
nez_expression* _nez_new_choice(nez_expression **expr, size_t size);
void nez_dispose_choice(nez_expression *expr);
#define nez_new_new(...)\
  (nez_expression[][]){ __VA_ARGS__ },\
  sizeof((nez_expression[][]){ __VA_ARGS__ }) / sizeof(nez_expression)\
  )
nez_expression* _nez_new_new(nez_expression **expr, size_t size);
void nez_dispose_new(nez_expression *expr);
#define nez_new_left_join(...)\
  (nez_expression[][]){ __VA_ARGS__ },\
  sizeof((nez_expression[][]){ __VA_ARGS__ }) / sizeof(nez_expression)\
  )
nez_expression* _nez_new_left_join(nez_expression **expr, size_t size);
void nez_dispose_left_join(nez_expression *expr);
nez_expression* nez_new_option(nez_expression *expr);
void nez_dispose_option(nez_expression *expr);
nez_expression* nez_new_repetition(nez_expression *expr);
void nez_dispose_repetition(nez_expression *expr);
nez_expression* nez_new_not(nez_expression *expr);
void nez_dispose_not(nez_expression *expr);
nez_expression* nez_new_and(nez_expression *expr);
void nez_dispose_and(nez_expression *expr);
nez_expression* nez_new_link(nez_expression *expr);
void nez_dispose_link(nez_expression *expr);
nez_expression* nez_new_tagging(nez_tag_entry* tag);
void nez_dispose_tagging(nez_expression *expr);
nez_expression* nez_new_value(char* value);
void nez_dispose_value(nez_expression *expr);
nez_expression* nez_new_char(char c);
void nez_dispose_char(nez_expression *expr);
nez_expression* nez_new_charset(char* charset);
void nez_dispose_charset(nez_expression *expr);
nez_expression* nez_new_string(char* str);
void nez_dispose_string(nez_expression *expr);
nez_expression* nez_new_any();
void nez_dispose_any(nez_expression *expr);
nez_expression* nez_new_non_terminal(char* rule_name);
void nez_dispose_non_terminal(nez_expression *expr);
nez_expression* nez_new_def(nez_expression *expr, nez_tag_entry* tag);
void nez_dispose_def(nez_expression *expr);
nez_expression* nez_new_defindent(nez_expression *expr);
void nez_dispose_defindent(nez_expression *expr);
nez_expression* nez_new_is(nez_expression *expr, nez_tag_entry* tag);
void nez_dispose_is(nez_expression *expr);
nez_expression* nez_new_isa(nez_expression *expr, nez_tag_entry* tag);
void nez_dispose_isa(nez_expression *expr);
nez_expression* nez_new_indent(nez_expression *expr);
void nez_dispose_indent(nez_expression *expr);
nez_expression* nez_new_if(nez_expression *expr, char* flag_name);
void nez_dispose_if(nez_expression *expr);
nez_expression* nez_new_with(nez_expression *expr, char* flag_name);
void nez_dispose_with(nez_expression *expr);
nez_expression* nez_new_without(nez_expression *expr, char* flag_name);
void nez_dispose_without(nez_expression *expr);

typedef void (*dispose_expr_func_t)(nez_expression *);
static dispose_expr_func_t f_dispose_expr[] = {
#define DEFINE_DISPOSE_EXPR_FUNC(EXPR) nez_dispose_##EXPR,
  NEZ_EXPR_EACH(DEFINE_DISPOSE_EXPR_FUNC)
};

char *load_file(const char *filename, size_t *length);

void nez_print_error_info(const char *errmsg);
nez_node* nez_parse(nez_context* context, nezvm_instruction* inst);

#endif /* end of include guard */