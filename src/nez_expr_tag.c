#include "nezvm.h"

static nez_tag_entry* TEXT;
static nez_tag_entry* INTEGER;
static nez_tag_entry* NAME;

static nez_tag_entry* ANYCHAR;
static nez_tag_entry* BYTECHAR;
static nez_tag_entry* CLASS;
static nez_tag_entry* CHARACTER;
static nez_tag_entry* STRING;
static nez_tag_entry* LIST;

static nez_tag_entry* NONTERMINAL;
static nez_tag_entry* CHOICE;
static nez_tag_entry* SEQUENCE;
static nez_tag_entry* REPETITION;
static nez_tag_entry* REPETITION1;
static nez_tag_entry* OPTION;
static nez_tag_entry* NOT;
static nez_tag_entry* AND;
static nez_tag_entry* RULE;
static nez_tag_entry* IMPORT;
static nez_tag_entry* ANNOTATION;

static nez_tag_entry* MATCH;
static nez_tag_entry* NEW;
static nez_tag_entry* LEFTNEW;
static nez_tag_entry* LINK;
static nez_tag_entry* TAGGING;
static nez_tag_entry* REPLACE;

static nez_tag_entry* IF;
static nez_tag_entry* WITHOUT;
static nez_tag_entry* WITH;

static nez_tag_entry* BLOCK;
static nez_tag_entry* DEF;
static nez_tag_entry* IS;
static nez_tag_entry* ISA;
static nez_tag_entry* DEFINDENT;
static nez_tag_entry* INDENT;

static nez_tag_entry* SCAN;
static nez_tag_entry* REPEAT;

int nez_new_expr_tag() {
	nez_expr_tag = nez_new_tag();
	TEXT         = tag(nez_expr_tag, "Text");
	INTEGER   	 = tag(nez_expr_tag, "Integer");
	NAME         = tag(nez_expr_tag, "Name");
	ANYCHAR      = tag(nez_expr_tag, "Any");
	BYTECHAR     = tag(nez_expr_tag, "Byte");
	CLASS        = tag(nez_expr_tag, "Class");
	CHARACTER    = tag(nez_expr_tag, "Character");
	STRING       = tag(nez_expr_tag, "String");
	LIST         = tag(nez_expr_tag, "List");
	NONTERMINAL  = tag(nez_expr_tag, "NonTerminal");
	CHOICE       = tag(nez_expr_tag, "Choice");
	SEQUENCE     = tag(nez_expr_tag, "Sequence");
	REPETITION   = tag(nez_expr_tag, "Repetition");
	REPETITION1  = tag(nez_expr_tag, "Repetition1");
	OPTION       = tag(nez_expr_tag, "Option");
	NOT          = tag(nez_expr_tag, "Not");
	AND          = tag(nez_expr_tag, "And");
	RULE         = tag(nez_expr_tag, "Rule");
	IMPORT       = tag(nez_expr_tag, "Import");
	ANNOTATION   = tag(nez_expr_tag, "Annotation");
	MATCH        = tag(nez_expr_tag, "Match");
	NEW          = tag(nez_expr_tag, "New");
	LEFTNEW      = tag(nez_expr_tag, "LeftNew");
	LINK         = tag(nez_expr_tag, "Link");
	TAGGING      = tag(nez_expr_tag, "Tagging");
	REPLACE      = tag(nez_expr_tag, "Replace");
	IF           = tag(nez_expr_tag, "If");
	WITHOUT      = tag(nez_expr_tag, "Without");
	WITH         = tag(nez_expr_tag, "With");
	BLOCK        = tag(nez_expr_tag, "Block");
	DEF          = tag(nez_expr_tag, "Def");
	IS           = tag(nez_expr_tag, "Is");
	ISA          = tag(nez_expr_tag, "Isa");
	DEFINDENT    = tag(nez_expr_tag, "DefIndent");
	INDENT       = tag(nez_expr_tag, "Indent");
	SCAN         = tag(nez_expr_tag, "Scan");
	REPEAT       = tag(nez_expr_tag, "Repeat");
}