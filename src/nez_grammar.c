#include "nezvm.h"

nez_grammar* nez_load_grammar(const char *syntax_file) {
	size_t len, rule_list_size;
	char *syntax_text = load_file(syntax_file, &len);
	nez_node *nez_ast = nez_parse_grammar(syntax_text);
	nez_rule *rule_list = nez_create_rules(&rule_list_size);
	return nez_create_grammar(syntax_file, rule_list, rule_list_size);
}

nez_grammar* nez_create_grammar(const char *syntax_file, nez_rule *rule_list, size_t rule_list_size) {
	nez_grammar* nez = (nez_grammar *)malloc(sizeof(nez_grammar));
	nez->input_file_name = syntax_file;
	nez->rule_list_size = rule_list_size;
	nez->rule_list = (nez_rule *)malloc(sizeof(nez_rule) * rule_list_size);
	return nez;
}

void nez_dispose_grammar(nez_grammar* nez) {
	free(nez->input_file_name);
	nez_dispose_rules(nez->rule_list, nez->rule_list_size);
	free(nez);
}

nez_rule* nez_create_rules(size_t* rule_list_size) {

}

void nez_dispose_rules(nez_rule *rule_list, size_t rule_list_size) {

}