#include <string.h>
#include <stdio.h>
#include "nezvm.h"

#define SEQUENCE nez_new_sequence
#define CHOICE nez_new_choice
#define NEW nez_new_new
#define LEFT_JOIN nez_new_left_join
#define OPTION nez_new_option
#define REPETITION nez_new_repetition
#define NOT nez_new_not
#define AND nez_new_and
#define LINK nez_new_link
#define LINK_IDX nez_new_link_idx
#define TAGGING nez_new_tagging
#define VALUE nez_new_value
#define CHAR nez_new_char
#define CHARSET nez_new_charset
#define STRING nez_new_string
#define ANY nez_new_any
#define P nez_new_non_terminal
#define DEF nez_new_def
#define DEFINDENT nez_new_defindent
#define IS nez_new_is
#define ISA nez_new_isa
#define INDENT nez_new_indent
#define IF nez_new_if
#define WITH nez_new_with
#define WITHOUT nez_new_without

#define NEZ_RULE_SIZE 38

nez_expression* EOL() {
	return CHOICE(STRING("\r"), STRING("\n"));
}

nez_expression* EOT() {
	return NOT(ANY());
}

nez_expression* SEMI() {
	return OPTION(STRING(";"));
}

nez_expression* S() {
	return CHOICE(CHARSET(" \\t\\r\\n"), STRING("\u3000"));
}

nez_expression* DIGIT() {
	return CHARSET("0-9");
}

nez_expression* LETTER() {
	return CHARSET("A-Za-z_");
}

nez_expression* HEX() {
	return CHARSET("0-9A-Fa-f");
}

nez_expression* W() {
	return CHARSET("A-Za-z0-9_");
}

nez_expression* INT() {
	return SEQUENCE(P("DIGIT"), REPETITION(P("DIGIT")));
}

nez_expression* NAME() {
	return SEQUENCE(P("LETTER"), REPETITION(P("W")));
}

nez_expression* COMMENT() {
	return CHOICE(
		SEQUENCE(STRING("/*"), REPETITION(SEQUENCE(NOT(STRING("*/")), ANY())), STRING("*/")),
		SEQUENCE(STRING("//"), REPETITION(SEQUENCE(NOT(P("EOL")), ANY())), P("EOL"))
	);
}

nez_expression* SPACING() {
	return REPETITION(CHOICE(P("S"), P("COMMENT")));
}

nez_expression* Int() {
	return NEW(P("INT"), TAGGING(tag(nez_expr_tag, "Integer")));
}

nez_expression* Name() {
	return NEW(P("LETTER"), REPETITION(P("W")), TAGGING(tag(nez_expr_tag, "Name")));
}

nez_expression* DotName() {
	return NEW(P("LETTER"), REPETITION(CHARSET("A-Za-z0-9_.")), TAGGING(tag(nez_expr_tag, "Name")));
}

nez_expression* HyphenName() {
	return NEW(P("LETTER"), REPETITION(CHOICE(P("W"), STRING("-"))), TAGGING(tag(nez_expr_tag, "Name")));
}

nez_expression* String() {
	nez_expression* StringContent  = REPETITION(CHOICE(
		STRING("\\\""), STRING("\\\\"), SEQUENCE(NOT(STRING("\"")), ANY())
	));
	return SEQUENCE(STRING("\""), NEW(StringContent, TAGGING(tag(nez_expr_tag, "String"))), STRING("\""));
}

nez_expression* SingleQuotedString() {
	nez_expression* StringContent  = REPETITION(CHOICE(
		STRING("\\'"), STRING("\\\\"), SEQUENCE(NOT(STRING("'")), ANY())
	));
	return SEQUENCE(STRING("'"),  NEW(StringContent, TAGGING(tag(nez_expr_tag, "CharacterSEQUENCE"))), STRING("'"));
}

nez_expression* ValueReplacement() {
	nez_expression* ValueContent = REPETITION(CHOICE(
		STRING("\\`"), STRING("\\\\"), SEQUENCE(NOT(STRING("`")), ANY())
	));
	return SEQUENCE(STRING("`"), NEW(ValueContent, TAGGING(tag(nez_expr_tag, "Value"))), STRING("`"));
}

nez_expression* NonTerminal() {
	return NEW(
			P("LETTER"), 
			REPETITION(CHARSET("A-Za-z0-9_:")), 
			TAGGING(tag(nez_expr_tag, "NonTerminal"))
	);
}

nez_expression* CHARACTER() {
	return CHOICE( 
		SEQUENCE(STRING("\\u"), P("HEX"), P("HEX"), P("HEX"), P("HEX")),
		SEQUENCE(STRING("\\x"), P("HEX"), P("HEX")),
		STRING("\\n"), STRING("\\t"), STRING("\\\\"), STRING("\\r"), STRING("\\v"), STRING("\\f"), STRING("\\-"), STRING("\\]"), 
		SEQUENCE(NOT(STRING("]")), ANY())
	);
}

nez_expression* Charset() {
	nez_expression* _CharChunk = SEQUENCE(
		NEW (P("CHAR"), TAGGING(tag(nez_expr_tag, "Character"))), 
		OPTION(
			LEFT_JOIN(STRING("-"), LINK(NEW(P("CHAR"), TAGGING(tag(nez_expr_tag, "Character")))), TAGGING(tag(nez_expr_tag, "List")))
		)
	);
	return SEQUENCE(STRING("["), NEW(REPETITION(LINK(_CharChunk)), TAGGING(tag(nez_expr_tag, "Character"))), STRING("]"));
}

nez_expression* Constructor() {
	return NEW(
		STRING("{"), 
		CHOICE(
			SEQUENCE(STRING("@"), P("S"), TAGGING(tag(nez_expr_tag, "LeftJoin"))), 
			TAGGING(tag(nez_expr_tag, "Constructor"))
		), 
		P("_"), 
		OPTION(SEQUENCE(LINK(P("Expr")), P("_"))),
		STRING("}")
	);
}

nez_expression* Func() {
	return SEQUENCE(STRING("<"), 
		NEW(CHOICE(
			SEQUENCE(STRING("match"),   P("S"), LINK(P("Expr")), P("_"), STRING(">"), TAGGING(tag(nez_expr_tag, "Match"))),
			SEQUENCE(STRING("if"), P("S"), OPTION(STRING("!")), LINK(P("Name")), TAGGING(tag(nez_expr_tag, "If"))),
			SEQUENCE(STRING("with"),  P("S"), LINK(P("Name")), P("S"), LINK(P("Expr")), TAGGING(tag(nez_expr_tag, "With"))),
			SEQUENCE(STRING("without"), P("S"), LINK(P("Name")), P("S"), LINK(P("Expr")), TAGGING(tag(nez_expr_tag, "Without"))),
			SEQUENCE(STRING("block"), OPTION(SEQUENCE(P("S"), LINK(P("Expr")))), TAGGING(tag(nez_expr_tag, "Block"))),
			SEQUENCE(STRING("indent"), TAGGING(tag(nez_expr_tag, "Indent"))),
			SEQUENCE(STRING("is"), P("S"), LINK(P("Name")), TAGGING(tag(nez_expr_tag, "Is"))),
			SEQUENCE(STRING("isa"), P("S"), LINK(P("Name")), TAGGING(tag(nez_expr_tag, "Isa"))),
			SEQUENCE(STRING("def"),  P("S"), LINK(P("Name")), P("S"), LINK(P("Expr")), TAGGING(tag(nez_expr_tag, "Def"))),
			SEQUENCE(STRING("scan"), P("S"), LINK(NEW(DIGIT(), REPETITION(DIGIT()))), STRING(","), P("S"), LINK(P("Expr")), STRING(","), P("S"), LINK(P("Expr")), TAGGING(tag(nez_expr_tag, "Scan"))),
			SEQUENCE(STRING("repeat"), P("S"), LINK(P("Expr")), TAGGING(tag(nez_expr_tag, "Repeat")))
		)), P("_"), STRING(">")
	);
}

nez_expression* Term() {
	nez_expression* _Any = NEW(STRING("."), TAGGING(tag(nez_expr_tag, "Any")));
	nez_expression* _TAGGING = SEQUENCE(STRING("#"), NEW(CHARSET("A-Za-z0-9"), REPETITION(CHARSET("A-Za-z0-9_.")), TAGGING(tag(nez_expr_tag, "TAGGING"))));
	nez_expression* _Byte = NEW(STRING("0x"), P("HEX"), P("HEX"), TAGGING(tag(nez_expr_tag, "Byte")));
	nez_expression* _Unicode = NEW(STRING("U+"), P("HEX"), P("HEX"), P("HEX"), P("HEX"), TAGGING(tag(nez_expr_tag, "Byte")));
	return CHOICE(
		P("SingleQuotedString"), 
		P("Charset"), 
		P("Func"),  
		_Any, 
		P("ValueReplacement"), 
		_TAGGING, 
		_Byte, 
		_Unicode,
		SEQUENCE(STRING("("), P("_"), P("Expr"), P("_"), STRING(")")),
		P("Constructor"), 
		P("String"), 
		P("NonTerminal") 
	);
}

nez_expression* SuffixTerm() {
	nez_expression* Connector  = CHOICE(STRING("@"), STRING("^"));
	return SEQUENCE(
		P("Term"), 
		OPTION(
			LEFT_JOIN(
				CHOICE(
					SEQUENCE(STRING("*"), OPTION(LINK_IDX(1, P("Int"))), TAGGING(tag(nez_expr_tag, "Repetition"))), 
					SEQUENCE(STRING("+"), TAGGING(tag(nez_expr_tag, "OneMoreRepetition"))), 
					SEQUENCE(STRING("?"), TAGGING(tag(nez_expr_tag, "OPTION"))),
					SEQUENCE(Connector, OPTION(LINK_IDX(1, P("Int"))), TAGGING(tag(nez_expr_tag, "Connector")))
				)
			)
		)
	);
}

nez_expression* Predicate() {
	return CHOICE(
		NEW(
			CHOICE(
				SEQUENCE(STRING("&"), TAGGING(tag(nez_expr_tag, "And"))),
				SEQUENCE(STRING("!"), TAGGING(tag(nez_expr_tag, "NOT"))),
				SEQUENCE(STRING("@["), P("_"), LINK_IDX(1, P("Int")), P("_"), STRING("]"), TAGGING(tag(nez_expr_tag, "Connector"))),							
				SEQUENCE(STRING("@"), TAGGING(tag(nez_expr_tag, "Connector")))
			), 
			LINK_IDX(0, P("SuffixTerm"))
		), 
		P("SuffixTerm")
	);
}

nez_expression* NOTRULE() {
	return NOT(CHOICE(STRING(";"), P("RuleHead"), P("Import")));
}

nez_expression* SEQ() {
	return SEQUENCE(
		P("Predicate"), 
		OPTION(
			LEFT_JOIN(
				P("_"), 
				P("NOTRULE"),
				LINK(P("Predicate")),
				REPETITION(SEQUENCE(
					P("_"), 
					P("NOTRULE"),
					LINK(P("Predicate"))
				)),
				TAGGING(tag(nez_expr_tag, "SEQUENCE")) 
			)
		)
	);
}

nez_expression* Expr() {
	return SEQUENCE(
		P("SEQUENCE"), 
		OPTION(
			LEFT_JOIN(
				P("_"), STRING("/"), P("_"), 
				LINK(P("SEQUENCE")), 
				REPETITION(SEQUENCE(
					P("_"), STRING("/"), P("_"), 
					LINK(P("SEQUENCE"))
				)),
				TAGGING(tag(nez_expr_tag, "CHOICE")) 
			)
		)
	);
}
	
nez_expression* DOC() {
	return SEQUENCE(
		REPETITION(SEQUENCE(NOT(STRING("]")), NOT(STRING("[")), ANY())),
		OPTION(SEQUENCE(STRING("["), P("DOC"), STRING("]"), P("DOC") ))
	);
}

nez_expression* Annotation() {
	return SEQUENCE(
		STRING("["),
		NEW(
			LINK(P("HyphenName")),
			OPTION(SEQUENCE(
				STRING(":"),  P("_"), 
				LINK(NEW(P("DOC"), TAGGING(tag(nez_expr_tag, "Text")))),
				TAGGING(tag(nez_expr_tag, "Annotation"))
			))
		),
		STRING("]"),
		P("_")
	);
}

nez_expression* Annotations() {
	return NEW(
		LINK(P("Annotation")),
		REPETITION(LINK(P("Annotation"))),
		TAGGING(tag(nez_expr_tag, "List")) 
	);	
}

nez_expression* Rule() {
	return NEW(
		LINK_IDX(0, CHOICE(P("Name"), P("String"))), P("_"), 
		OPTION(SEQUENCE(LINK_IDX(2, P("Annotations")), P("_"))),
		STRING("="), P("_"), 
		LINK_IDX(1, P("Expr")),
		TAGGING(tag(nez_expr_tag, "Rule")) 
	);
}

nez_expression* RuleHead() {
	return SEQUENCE(
		LINK_IDX(0, CHOICE(P("Name"), P("String"))), P("_"), 
		OPTION(SEQUENCE(LINK_IDX(2, P("Annotations")), P("_"))),
		STRING("="), 
		TAGGING(tag(nez_expr_tag, "Rule")) 
	);
}

nez_expression* Import() {
	return NEW(
		STRING("import"), P("S"), 
		LINK(P("NonTerminal")),
		REPETITION(SEQUENCE(P("_"), STRING(","), P("_"),  LINK(P("NonTerminal")))), P("_"), 
		STRING("from"), P("S"), 
		LINK(CHOICE(P("SingleQuotedString"), P("String"), P("DotName"))), 
		TAGGING(tag(nez_expr_tag, "Import"))
	);
}

nez_expression* Chunk() {
	return SEQUENCE(
		P("_"), 
		CHOICE(
			P("Rule"), 
			P("Import")
		), 
		P("_"), 
		OPTION(SEQUENCE(STRING(";"), P("_")))
	);
}

nez_expression* File() {
	return NEW(
		P("_"), 
		REPETITION(LINK(P("Chunk"))),
		TAGGING(tag(nez_expr_tag, "List"))
	);
}

#define NEZ_EOL 0
#define NEZ_EOT 1
#define NEZ_SEMI 2
#define NEZ_S 3
#define NEZ_DIGIT 4
#define NEZ_LETTER 5
#define NEZ_HEX 6
#define NEZ_W 7
#define NEZ_INT 8
#define NEZ_NAME 9
#define NEZ_COMMENT 10
#define NEZ_SPACING 11
#define NEZ_Int 12
#define NEZ_Name 13
#define NEZ_DotName 14
#define NEZ_HyphenName 15
#define NEZ_String 16
#define NEZ_SingleQuotedString 17
#define NEZ_ValueReplacement 18
#define NEZ_NonTerminal 19
#define NEZ_CHARACTER 20
#define NEZ_Charset 21
#define NEZ_Constructor 22
#define NEZ_Func 23
#define NEZ_Term 24
#define NEZ_SuffixTerm 25
#define NEZ_Predicate 26
#define NEZ_NOTRULE 27
#define NEZ_SEQ 28
#define NEZ_Expr 29
#define NEZ_DOC 30
#define NEZ_Annotation 31
#define NEZ_Annotations 32
#define NEZ_Rule 33
#define NEZ_RuleHead 34
#define NEZ_Import 35
#define NEZ_Chunk 36
#define NEZ_File 37

nez_rule** nez_load_nez_parser() {
	nez_new_expr_tag();
	size_t rule_size = NEZ_RULE_SIZE;
	nez_rule** rules = nez_create_rules(rule_size);
	rules[NEZ_EOL]->rule_name = "EOL";
	rules[NEZ_EOL]->expr = EOL();
	rules[NEZ_EOT]->rule_name = "EOT";
	rules[NEZ_EOT]->expr = EOT();
	rules[NEZ_SEMI]->rule_name = "SEMI";
	rules[NEZ_SEMI]->expr = SEMI();
	rules[NEZ_S]->rule_name = "S";
	rules[NEZ_S]->expr = S();
	rules[NEZ_DIGIT]->rule_name = "DIGIT";
	rules[NEZ_DIGIT]->expr = DIGIT();
	rules[NEZ_LETTER]->rule_name = "LETTER";
	rules[NEZ_LETTER]->expr = LETTER();
	rules[NEZ_HEX]->rule_name = "HEX";
	rules[NEZ_HEX]->expr = HEX();
	rules[NEZ_W]->rule_name = "W";
	rules[NEZ_W]->expr = W();
	rules[NEZ_INT]->rule_name = "INT";
	rules[NEZ_INT]->expr = INT();
	rules[NEZ_NAME]->rule_name = "NAME";
	rules[NEZ_NAME]->expr = NAME();
	rules[NEZ_COMMENT]->rule_name = "COMMENT";
	rules[NEZ_COMMENT]->expr = COMMENT();
	rules[NEZ_SPACING]->rule_name = "SPACING";
	rules[NEZ_SPACING]->expr = SPACING();
	rules[NEZ_Int]->rule_name = "Int";
	rules[NEZ_Int]->expr = Int();
	rules[NEZ_Name]->rule_name = "Name";
	rules[NEZ_Name]->expr = Name();
	rules[NEZ_DotName]->rule_name = "DotName";
	rules[NEZ_DotName]->expr = DotName();
	rules[NEZ_HyphenName]->rule_name = "HyphenName";
	rules[NEZ_HyphenName]->expr = HyphenName();
	rules[NEZ_String]->rule_name = "String";
	rules[NEZ_String]->expr = String();
	rules[NEZ_SingleQuotedString]->rule_name = "SingleQuotedString";
	rules[NEZ_SingleQuotedString]->expr = SingleQuotedString();
	rules[NEZ_ValueReplacement]->rule_name = "ValueReplacement";
	rules[NEZ_ValueReplacement]->expr = ValueReplacement();
	rules[NEZ_NonTerminal]->rule_name = "NonTerminal";
	rules[NEZ_NonTerminal]->expr = NonTerminal();
	rules[NEZ_CHARACTER]->rule_name = "CHARACTER";
	rules[NEZ_CHARACTER]->expr = CHARACTER();
	rules[NEZ_Charset]->rule_name = "Charset";
	rules[NEZ_Charset]->expr = Charset();
	rules[NEZ_Constructor]->rule_name = "Constructor";
	rules[NEZ_Constructor]->expr = Constructor();
	rules[NEZ_Func]->rule_name = "Func";
	rules[NEZ_Func]->expr = Func();
	rules[NEZ_Term]->rule_name = "Term";
	rules[NEZ_Term]->expr = Term();
	rules[NEZ_SuffixTerm]->rule_name = "SuffixTerm";
	rules[NEZ_SuffixTerm]->expr = SuffixTerm();
	rules[NEZ_Predicate]->rule_name = "Predicate";
	rules[NEZ_Predicate]->expr = Predicate();
	rules[NEZ_NOTRULE]->rule_name = "NOTRULE";
	rules[NEZ_NOTRULE]->expr = NOTRULE();
	rules[NEZ_SEQ]->rule_name = "SEQ";
	rules[NEZ_SEQ]->expr = SEQ();
	rules[NEZ_Expr]->rule_name = "Expr";
	rules[NEZ_Expr]->expr = Expr();
	rules[NEZ_DOC]->rule_name = "DOC";
	rules[NEZ_DOC]->expr = DOC();
	rules[NEZ_Annotation]->rule_name = "Annotation";
	rules[NEZ_Annotation]->expr = Annotation();
	rules[NEZ_Annotations]->rule_name = "Annotations";
	rules[NEZ_Annotations]->expr = Annotations();
	rules[NEZ_Rule]->rule_name = "Rule";
	rules[NEZ_Rule]->expr = Rule();
	rules[NEZ_RuleHead]->rule_name = "RuleHead";
	rules[NEZ_RuleHead]->expr = RuleHead();
	rules[NEZ_Import]->rule_name = "Import";
	rules[NEZ_Import]->expr = Import();
	rules[NEZ_Chunk]->rule_name = "Chunk";
	rules[NEZ_Chunk]->expr = Chunk();
	rules[NEZ_File]->rule_name = "File";
	rules[NEZ_File]->expr = File();
	return rules;
}