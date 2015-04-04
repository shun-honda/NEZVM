#include "nezvm.h"
#include <stdio.h>

void nez_dump_rules(nez_rule** rules) {
	for(int i = 0; i < NEZ_RULE_SIZE; i++) {
		fprintf(stderr, "%s =\n\t", rules[i]->rule_name);
		nez_dump_expression(rules[i]->expr);
		fprintf(stderr, "\n");
	}
}

static void nez_dump_charset(char c) {
	switch(c) {
		case '\n' : fprintf(stderr, "\\n"); break;
		case '\t' : fprintf(stderr, "\\t"); break;
		case '\r' : fprintf(stderr, "\\r"); break;
		case '\'' : fprintf(stderr, "\\'"); break;
		case ']' : fprintf(stderr, "\\]"); break;
		case '-' : fprintf(stderr, "\\-"); break;
		case '\\' : fprintf(stderr, "\\\\"); break;
		default:
			if(c > 127) {
				fprintf(stderr, "\\x%02x", (int)c);
			}
			else {
				fprintf(stderr, "%c", c);
			}
	}
}

void nez_dump_expression(nez_expression* expr) {
	switch (expr->base.expr) {
		case NEZ_SEQUENCE:{
			nez_sequence *e = (nez_sequence*)expr;
			for(size_t i = 0; i < e->base.size; i++) {
				if(i > 0) {
					fprintf(stderr, " ");
				}
				nez_expression* inner = e->list[i];
				if(inner->base.expr == NEZ_CHOICE || inner->base.expr == NEZ_SEQUENCE) {
					fprintf(stderr, "( ");
					nez_dump_expression(inner);
					fprintf(stderr, " )");
				}
				else {
					nez_dump_expression(inner);
				}
			}
			break;
		}
		case NEZ_CHOICE:{
			nez_choice *e = (nez_choice*)expr;
			for(size_t i = 0; i < e->base.size; i++) {
				if(i > 0) {
					fprintf(stderr, " / ");
				}
				nez_expression* inner = e->list[i];
				if(inner->base.expr == NEZ_CHOICE || inner->base.expr == NEZ_SEQUENCE) {
					fprintf(stderr, "( ");
					nez_dump_expression(inner);
					fprintf(stderr, " )");
				}
				else {
					nez_dump_expression(inner);
				}
			}
			break;
		}
		case NEZ_NEW:{
			nez_new *e = (nez_new*)expr;
			fprintf(stderr, "{ ");
			for(size_t i = 0; i < e->base.size; i++) {
				if(i > 0) {
					fprintf(stderr, " ");
				}
				nez_expression* inner = e->list[i];
				if(inner->base.expr == NEZ_CHOICE || inner->base.expr == NEZ_SEQUENCE) {
					fprintf(stderr, "( ");
					nez_dump_expression(inner);
					fprintf(stderr, " )");
				}
				else {
					nez_dump_expression(inner);
				}
			}
			fprintf(stderr, "}");
			break;
		}
		case NEZ_LEFT_JOIN:{
			nez_left_join *e = (nez_left_join*)expr;
			fprintf(stderr, "{@ ");
			for(size_t i = 0; i < e->base.size; i++) {
				if(i > 0) {
					fprintf(stderr, " ");
				}
				nez_expression* inner = e->list[i];
				if(inner->base.expr == NEZ_CHOICE || inner->base.expr == NEZ_SEQUENCE) {
					fprintf(stderr, "( ");
					nez_dump_expression(inner);
					fprintf(stderr, " )");
				}
				else {
					nez_dump_expression(inner);
				}
			}
			fprintf(stderr, "}\n");
			break;
		}
		case NEZ_OPTION:{
			nez_option *e = (nez_option*)expr;
			nez_dump_expression(e->inner);
			fprintf(stderr, "?");
			break;
		}
		case NEZ_REPETITION:{
			nez_repetition *e = (nez_repetition*)expr;
			nez_dump_expression(e->inner);
			fprintf(stderr, "*");
			break;
		}
		case NEZ_NOT:{
			nez_not *e = (nez_not*)expr;
			fprintf(stderr, "!");
			nez_dump_expression(e->inner);
			break;
		}
		case NEZ_AND:{
			nez_and *e = (nez_and*)expr;
			fprintf(stderr, "&");
			nez_dump_expression(e->inner);
			break;
		}
		case NEZ_LINK:{
			nez_link *e = (nez_link*)expr;
			fprintf(stderr, "@");
			if(e->index) {
				fprintf(stderr, "[%d]", e->index);
			}
			nez_dump_expression(e->inner);
			break;
		}
		case NEZ_TAGGING:{
			nez_tagging *e = (nez_tagging*)expr;
			fprintf(stderr, "#%s", e->tag->tag_name);
			break;
		}
		case NEZ_VALUE:{
			nez_value *e = (nez_value*)expr;
			fprintf(stderr, "`%s`", e->value);
			break;
		}
		case NEZ_CHAR:{
			nez_char *e = (nez_char*)expr;
			fprintf(stderr, "'");
			nez_dump_charset(e->c);
			fprintf(stderr, "'");
			break;
		}
		case NEZ_CHARSET:{
			nez_charset *e = (nez_charset*)expr;
			fprintf(stderr, "[");
			int i, j;
			for(i = 0; i < 256; i++) {
				if(bitset_get(e->set, i)) {
					for(j = i; j < 256; j++) {
						if(!bitset_get(e->set, j)) {
							break;
						}
					}
					if(i == j) {
						nez_dump_charset((char)i);
					}
					else {
						nez_dump_charset((char)i);
						fprintf(stderr, "-");
						nez_dump_charset((char)i);
					}
				}
			}
			fprintf(stderr, "]");
			break;
		}
		case NEZ_STRING:{
			nez_string *e = (nez_string*)expr;
			fprintf(stderr, "'");
			int len = strlen(e->str);
			for (int i = 0; i < len; i++) {
				nez_dump_charset(e->str[i]);
			}
			fprintf(stderr, "'");
			break;
		}
		case NEZ_ANY:{
			nez_any *e = (nez_any*)expr;
			fprintf(stderr, ".");
			break;
		}
		case NEZ_NON_TERMINAL:{
			nez_non_terminal *e = (nez_non_terminal*)expr;
			fprintf(stderr, "%s", e->rule_name);
			break;
		}
		case NEZ_DEF:{
			nez_def *e = (nez_def*)expr;
			fprintf(stderr, "def\n");
			nez_dump_expression(e->inner);
			break;
		}
		case NEZ_DEFINDENT:{
			nez_defindent *e = (nez_defindent*)expr;
			fprintf(stderr, "defindent\n");
			break;
		}
		case NEZ_IS:{
			nez_is *e = (nez_is*)expr;
			fprintf(stderr, "is\n");
			break;
		}
		case NEZ_ISA:{
			nez_isa *e = (nez_isa*)expr;
			fprintf(stderr, "isa\n");
			break;
		}
		case NEZ_INDENT:{
			nez_indent *e = (nez_indent*)expr;
			fprintf(stderr, "indent\n");
			break;
		}
		case NEZ_IF:{
			nez_if *e = (nez_if*)expr;
			fprintf(stderr, "if\n");
			break;
		}
		case NEZ_WITH:{
			nez_with *e = (nez_with*)expr;
			fprintf(stderr, "with\n");
			nez_dump_expression(e->inner);
			break;
		}
		case NEZ_WITHOUT:{
			nez_without *e = (nez_without*)expr;
			fprintf(stderr, "without\n");
			nez_dump_expression(e->inner);
			break;
		}
	}
}