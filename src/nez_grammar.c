#include "nezvm.h"

nez_grammar* nez_load_grammar(const char *syntax_file) {
	size_t len;
	char *syntax_text = load_file(syntax_file, &len);
	nez_node *nez_ast = nez_parse_grammar(syntax_text);
	return nez_create_grammar(nez_ast);
}

nez_grammar* nez_create_grammar(nez_node* nez_ast) {
	nez_grammar* nez = (nez_grammar *)malloc(sizeof(nez_grammar));
	return nez;
}

void nez_dispose_grammar(nez_grammar* nez) {
	nez_dispose_rules(nez->rule_list, nez->rule_list_size);
	free(nez);
}

nez_rule** nez_create_rules(size_t rule_list_size) {
	nez_rule** rules = malloc(sizeof(nez_rule*) * rule_list_size);
	for(size_t i = 0; i < rule_list_size; i++) {
		rules[i] = (nez_rule*)malloc(sizeof(nez_rule));
	}
	return rules;
}

void nez_dispose_rules(nez_rule **rules, size_t rule_list_size) {
	for(size_t i = 0; i < rule_list_size; i++) {
		f_dispose_expr[rules[i]->expr->base.expr](rules[i]->expr);
		free(rules[i]);
	}
	free(rules);
}