#include "nezvm.h"
#include <string.h>

nez_node* nez_parse_grammar(char *syntax) {
	nez_rule** rules = nez_load_nez_parser();
	nez_dump_rules(rules);
	nez_dispose_rules(rules, NEZ_RULE_SIZE);
	nez_dispose_tag(nez_expr_tag);
	exit(EXIT_FAILURE);
	return NULL;
}

nez_expression* _nez_new_sequence(nez_expression **expr, size_t size) {
	nez_sequence *e = (nez_sequence*)malloc(sizeof(nez_sequence));
	e->base.expr = NEZ_SEQUENCE;
	e->base.size = size;
	e->list = (nez_expression**) malloc(sizeof(nez_expression*)*size);
	for(size_t i = 0; i < size; i++) {
		e->list[i] = expr[i];
	}
	return (nez_expression *)e;
}

void nez_dispose_sequence(nez_expression *expr) {
	nez_sequence* e = (nez_sequence*)expr;
	for(size_t i = 0; i < e->base.size; i++) {
		f_dispose_expr[e->list[i]->base.expr](e->list[i]);
	}
	free(e->list);
	free(e);
}

nez_expression* _nez_new_choice(nez_expression **expr, size_t size) {
	nez_choice *e = (nez_choice*)malloc(sizeof(nez_choice));
	e->base.expr = NEZ_CHOICE;
	e->base.size = size;
	e->list = (nez_expression**) malloc(sizeof(nez_expression*)*size);
	for(size_t i = 0; i < size; i++) {
		e->list[i] = expr[i];
	}
	return (nez_expression *)e;
}

void nez_dispose_choice(nez_expression *expr) {
	nez_choice* e = (nez_choice*)expr;
	for(size_t i = 0; i < e->base.size; i++) {
		f_dispose_expr[e->list[i]->base.expr](e->list[i]);
	}
	free(e->list);
	free(e);
}

nez_expression* _nez_new_new(nez_expression **expr, size_t size) {
	nez_new *e = (nez_new*)malloc(sizeof(nez_new));
	e->base.expr = NEZ_NEW;
	e->base.size = size;
	e->list = (nez_expression**) malloc(sizeof(nez_expression*)*size);
	for(size_t i = 0; i < size; i++) {
		e->list[i] = expr[i];
	}
	return (nez_expression *)e;
}

void nez_dispose_new(nez_expression *expr) {
	nez_new* e = (nez_new*)expr;
	for(size_t i = 0; i < e->base.size; i++) {
		f_dispose_expr[e->list[i]->base.expr](e->list[i]);
	}
	free(e->list);
	free(e);
}

nez_expression* _nez_new_left_join(nez_expression **expr, size_t size) {
	nez_left_join *e = (nez_left_join*)malloc(sizeof(nez_left_join));
	e->base.expr = NEZ_LEFT_JOIN;
	e->base.size = size;
	e->list = (nez_expression**) malloc(sizeof(nez_expression*)*size);
	for(size_t i = 0; i < size; i++) {
		e->list[i] = expr[i];
	}
	return (nez_expression *)e;
}

void nez_dispose_left_join(nez_expression *expr) {
	nez_left_join* e = (nez_left_join*)expr;
	for(size_t i = 0; i < e->base.size; i++) {
		f_dispose_expr[e->list[i]->base.expr](e->list[i]);
	}
	free(e->list);
	free(e);
}

nez_expression* nez_new_option(nez_expression *expr) {
	nez_option *e = (nez_option*)malloc(sizeof(nez_option));
	e->base.expr = NEZ_OPTION;
	e->base.size = 1;
	e->inner = expr;
	return (nez_expression *)e;
}

void nez_dispose_option(nez_expression *expr) {
	nez_option* e = (nez_option*)expr;
	f_dispose_expr[e->inner->base.expr](e->inner);
	free(e);
}

nez_expression* nez_new_repetition(nez_expression *expr) {
	nez_repetition *e = (nez_repetition*)malloc(sizeof(nez_repetition));
	e->base.expr = NEZ_REPETITION;
	e->base.size = 1;
	e->inner = expr;
	return (nez_expression *)e;
}

void nez_dispose_repetition(nez_expression *expr) {
	nez_repetition* e = (nez_repetition*)expr;
	f_dispose_expr[e->inner->base.expr](e->inner);
	free(e);
}

nez_expression* nez_new_not(nez_expression *expr) {
	nez_not *e = (nez_not*)malloc(sizeof(nez_not));
	e->base.expr = NEZ_NOT;
	e->base.size = 1;
	e->inner = expr;
	return (nez_expression *)e;
}

void nez_dispose_not(nez_expression *expr) {
	nez_not* e = (nez_not*)expr;
	f_dispose_expr[e->inner->base.expr](e->inner);
	free(e);
}

nez_expression* nez_new_and(nez_expression *expr) {
	nez_and *e = (nez_and*)malloc(sizeof(nez_and));
	e->base.expr = NEZ_AND;
	e->base.size = 1;
	e->inner = expr;
	return (nez_expression *)e;
}

void nez_dispose_and(nez_expression *expr) {
	nez_and* e = (nez_and*)expr;
	f_dispose_expr[e->inner->base.expr](e->inner);
	free(e);
}

nez_expression* nez_new_link(nez_expression *expr) {
	nez_link *e = (nez_link*)malloc(sizeof(nez_link));
	e->base.expr = NEZ_LINK;
	e->base.size = 1;
	e->inner = expr;
	return (nez_expression *)e;
}

nez_expression* nez_new_link_idx(int i, nez_expression *expr) {
	nez_link *e = (nez_link*)malloc(sizeof(nez_link));
	e->base.expr = NEZ_LINK;
	e->base.size = 1;
	e->inner = expr;
	e->index = i;
	return (nez_expression *)e;
}

void nez_dispose_link(nez_expression *expr) {
	nez_link* e = (nez_link*)expr;
	f_dispose_expr[e->inner->base.expr](e->inner);
	free(e);
}

nez_expression* nez_new_tagging(nez_tag_entry* tag) {
	nez_tagging *e = (nez_tagging*)malloc(sizeof(nez_tagging));
	e->base.expr = NEZ_TAGGING;
	e->base.size = 0;
	e->tag = tag;
	return (nez_expression *)e;
}

void nez_dispose_tagging(nez_expression *expr) {
	nez_tagging* e = (nez_tagging*)expr;
	free(e);
}

nez_expression* nez_new_value(char* value) {
	nez_value *e = (nez_value*)malloc(sizeof(nez_value));
	e->base.expr = NEZ_VALUE;
	e->base.size = 0;
	e->value = value;
	return (nez_expression *)e;
}

void nez_dispose_value(nez_expression *expr) {
	free(expr);
}

nez_expression* nez_new_char(char c) {
	nez_char *e = (nez_char*)malloc(sizeof(nez_char));
	e->base.expr = NEZ_CHAR;
	e->base.size = 0;
	e->c = c;
	return (nez_expression *)e;
}

void nez_dispose_char(nez_expression *expr) {
	free(expr);
}

nez_expression* nez_new_charset(char* charset) {
	int len = strlen(charset);
	nez_charset *e = (nez_charset*)malloc(sizeof(nez_charset));
	e->base.expr = NEZ_CHARSET;
	e->base.size = 0;
	e->set = (bitset_ptr_t)malloc(sizeof(bitset_t));
  bitset_init(e->set);
  for (int i = 0; i < len; i++) {
    unsigned c = charset[i];
    bitset_set(e->set, c);
  }
	return (nez_expression *)e;
}

void nez_dispose_charset(nez_expression *expr) {
	nez_charset* e = (nez_charset*)expr;
	free(e->set);
	free(e);
}

nez_expression* nez_new_string(char* str) {
	nez_string *e = (nez_string*)malloc(sizeof(nez_string));
	e->base.expr = NEZ_STRING;
	e->base.size = 0;
	e->str = str;
	return (nez_expression *)e;
}

void nez_dispose_string(nez_expression *expr) {
	nez_string* e = (nez_string*)expr;
	free(e);
}

nez_expression* nez_new_any() {
	nez_any *e = (nez_any*)malloc(sizeof(nez_any));
	e->base.expr = NEZ_ANY;
	e->base.size = 0;
	return (nez_expression *)e;
}

void nez_dispose_any(nez_expression *expr) {
	free(expr);
}

nez_expression* nez_new_non_terminal(char* rule_name) {
	nez_non_terminal *e = (nez_non_terminal*)malloc(sizeof(nez_non_terminal));
	e->base.expr = NEZ_NON_TERMINAL;
	e->base.size = 0;
	e->rule_name = rule_name;
	return (nez_expression *)e;
}

void nez_dispose_non_terminal(nez_expression *expr) {
	nez_non_terminal* e = (nez_non_terminal*)expr;
	free(e);
}

nez_expression* nez_new_def(nez_expression *expr, nez_tag_entry* tag) {
	nez_def *e = (nez_def*)malloc(sizeof(nez_def));
	e->base.expr = NEZ_DEF;
	e->base.size = 1;
	e->inner = expr;
	e->table = tag;
	return (nez_expression *)e;
}

void nez_dispose_def(nez_expression *expr) {
	nez_def* e = (nez_def*)expr;
	f_dispose_expr[e->inner->base.expr](e->inner);
	free(e->table);
	free(e);
}

nez_expression* nez_new_defindent() {
	nez_defindent *e = (nez_defindent*)malloc(sizeof(nez_defindent));
	e->base.expr = NEZ_DEFINDENT;
	e->base.size = 0;
	return (nez_expression *)e;
}

void nez_dispose_defindent(nez_expression *expr) {
	free(expr);
}

nez_expression* nez_new_is(nez_expression *expr, nez_tag_entry* tag) {
	nez_is *e = (nez_is*)malloc(sizeof(nez_is));
	e->base.expr = NEZ_IS;
	e->base.size = 0;
	e->table = tag;
	return (nez_expression *)e;
}

void nez_dispose_is(nez_expression *expr) {
	free(expr);
}

nez_expression* nez_new_isa(nez_expression *expr, nez_tag_entry* tag) {
	nez_isa *e = (nez_isa*)malloc(sizeof(nez_isa));
	e->base.expr = NEZ_ISA;
	e->base.size = 0;
	e->table = tag;
	return (nez_expression *)e;
}

void nez_dispose_isa(nez_expression *expr) {
	free(expr);
}

nez_expression* nez_new_indent(nez_expression *expr) {
	nez_indent *e = (nez_indent*)malloc(sizeof(nez_indent));
	e->base.expr = NEZ_INDENT;
	e->base.size = 0;
	return (nez_expression *)e;
}

void nez_dispose_indent(nez_expression *expr) {
	free(expr);
}

nez_expression* nez_new_if(nez_expression *expr, char* flag_name) {
	nez_if *e = (nez_if*)malloc(sizeof(nez_if));
	e->base.expr = NEZ_IF;
	e->base.size = 0;
	e->flag_name = flag_name;
	return (nez_expression *)e;
}

void nez_dispose_if(nez_expression *expr) {
	free(expr);
}

nez_expression* nez_new_with(nez_expression *expr, char* flag_name) {
	nez_with *e = (nez_with*)malloc(sizeof(nez_with));
	e->base.expr = NEZ_WITH;
	e->base.size = 1;
	e->inner = expr;
	e->flag_name = flag_name;
	return (nez_expression *)e;
}

void nez_dispose_with(nez_expression *expr) {
	nez_with* e = (nez_with*)expr;
	f_dispose_expr[e->inner->base.expr](e->inner);
	free(e);
}

nez_expression* nez_new_without(nez_expression *expr, char* flag_name) {
	nez_without *e = (nez_without*)malloc(sizeof(nez_without));
	e->base.expr = NEZ_WITHOUT;
	e->base.size = 1;
	e->inner = expr;
	e->flag_name = flag_name;
	return (nez_expression *)e;
}

void nez_dispose_without(nez_expression *expr) {
	nez_without* e = (nez_without*)expr;
	f_dispose_expr[e->inner->base.expr](e->inner);
	free(e);
}
